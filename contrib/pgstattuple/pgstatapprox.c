/*-------------------------------------------------------------------------
 *
 * pgstatapprox.c
 *		  Bloat estimation functions
 *
 * Copyright (c) 2014-2017, PostgreSQL Global Development Group
 *
 * This source code file contains modifications made by THL A29 Limited ("Tencent Modifications").
 * All Tencent Modifications are Copyright (C) 2023 THL A29 Limited.
 *
 * IDENTIFICATION
 *		  contrib/pgstattuple/pgstatapprox.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/visibilitymap.h"
#include "access/transam.h"
#include "access/xact.h"
#include "access/multixact.h"
#include "access/htup_details.h"
#include "catalog/namespace.h"
#include "funcapi.h"
#include "miscadmin.h"
#include "storage/bufmgr.h"
#include "storage/freespace.h"
#include "storage/procarray.h"
#include "storage/lmgr.h"
#include "utils/builtins.h"
#include "utils/tqual.h"
#include "commands/vacuum.h"

PG_FUNCTION_INFO_V1(pgstattuple_approx);
PG_FUNCTION_INFO_V1(pgstattuple_approx_v1_5);

Datum		pgstattuple_approx_internal(Oid relid, FunctionCallInfo fcinfo);

typedef struct output_type
{
	uint64		table_len;
	uint64		scanned_percent;
	uint64		tuple_count;
	uint64		tuple_len;
	double		tuple_percent;
	uint64		dead_tuple_count;
	uint64		dead_tuple_len;
	double		dead_tuple_percent;
	uint64		free_space;
	double		free_percent;
} output_type;

#define NUM_OUTPUT_COLUMNS 10

/*
 * This function takes an already open relation and scans its pages,
 * skipping those that have the corresponding visibility map bit set.
 * For pages we skip, we find the free space from the free space map
 * and approximate tuple_len on that basis. For the others, we count
 * the exact number of dead tuples etc.
 *
 * This scan is loosely based on vacuumlazy.c:lazy_scan_heap(), but
 * we do not try to avoid skipping single pages.
 */
static void
statapprox_heap(Relation rel, output_type *stat)
{
	BlockNumber scanned,
				nblocks,
				blkno;
	Buffer		vmbuffer = InvalidBuffer;
	BufferAccessStrategy bstrategy;
	TransactionId OldestXmin;

	OldestXmin = GetOldestXmin(rel, PROCARRAY_FLAGS_VACUUM);
	bstrategy = GetAccessStrategy(BAS_BULKREAD);

	nblocks = RelationGetNumberOfBlocks(rel);
	scanned = 0;

	for (blkno = 0; blkno < nblocks; blkno++)
	{
		Buffer		buf;
		Page		page;
		OffsetNumber offnum,
					maxoff;
		Size		freespace;

		CHECK_FOR_INTERRUPTS();

		/*
		 * If the page has only visible tuples, then we can find out the free
		 * space from the FSM and move on.
		 */
		if (VM_ALL_VISIBLE(rel, blkno, &vmbuffer))
		{
			freespace = GetRecordedFreeSpace(rel, blkno);
			stat->tuple_len += BLCKSZ - freespace;
			stat->free_space += freespace;
			continue;
		}

		buf = ReadBufferExtended(rel, MAIN_FORKNUM, blkno,
								 RBM_NORMAL, bstrategy);

		LockBuffer(buf, BUFFER_LOCK_SHARE);

		page = BufferGetPage(buf);

		/*
		 * It's not safe to call PageGetHeapFreeSpace() on new pages, so we
		 * treat them as being free space for our purposes.
		 */
		if (!PageIsNew(page))
			stat->free_space += PageGetHeapFreeSpace(page);
		else
			stat->free_space += BLCKSZ - SizeOfPageHeaderData;

		/* We may count the page as scanned even if it's new/empty */
		scanned++;

		if (PageIsNew(page) || PageIsEmpty(page))
		{
			UnlockReleaseBuffer(buf);
			continue;
		}

		/*
		 * Look at each tuple on the page and decide whether it's live or
		 * dead, then count it and its size. Unlike lazy_scan_heap, we can
		 * afford to ignore problems and special cases.
		 */
		maxoff = PageGetMaxOffsetNumber(page);

		for (offnum = FirstOffsetNumber;
			 offnum <= maxoff;
			 offnum = OffsetNumberNext(offnum))
		{
			ItemId		itemid;
			HeapTupleData tuple;

			itemid = PageGetItemId(page, offnum);

			if (!ItemIdIsUsed(itemid) || ItemIdIsRedirected(itemid) ||
				ItemIdIsDead(itemid))
			{
				continue;
			}

			Assert(ItemIdIsNormal(itemid));

			ItemPointerSet(&(tuple.t_self), blkno, offnum);

			tuple.t_data = (HeapTupleHeader) PageGetItem(page, itemid);
			tuple.t_len = ItemIdGetLength(itemid);
			tuple.t_tableOid = RelationGetRelid(rel);

			/*
			 * We follow VACUUM's lead in counting INSERT_IN_PROGRESS tuples
			 * as "dead" while DELETE_IN_PROGRESS tuples are "live".  We don't
			 * bother distinguishing tuples inserted/deleted by our own
			 * transaction.
			 */
			switch (HeapTupleSatisfiesVacuum(&tuple, OldestXmin, buf))
			{
				case HEAPTUPLE_LIVE:
				case HEAPTUPLE_DELETE_IN_PROGRESS:
					stat->tuple_len += tuple.t_len;
					stat->tuple_count++;
					break;
				case HEAPTUPLE_DEAD:
				case HEAPTUPLE_RECENTLY_DEAD:
				case HEAPTUPLE_INSERT_IN_PROGRESS:
					stat->dead_tuple_len += tuple.t_len;
					stat->dead_tuple_count++;
					break;
				default:
					elog(ERROR, "unexpected HeapTupleSatisfiesVacuum result");
					break;
			}
		}

		UnlockReleaseBuffer(buf);
	}

	stat->table_len = (uint64) nblocks * BLCKSZ;

	/*
	 * We don't know how many tuples are in the pages we didn't scan, so
	 * extrapolate the live-tuple count to the whole table in the same way
	 * that VACUUM does.  (Like VACUUM, we're not taking a random sample, so
	 * just extrapolating linearly seems unsafe.)  There should be no dead
	 * tuples in all-visible pages, so no correction is needed for that, and
	 * we already accounted for the space in those pages, too.
	 */
	stat->tuple_count = vac_estimate_reltuples(rel, false, nblocks, scanned,
	                                           stat->tuple_count);

	/*
	 * Calculate percentages if the relation has one or more pages.
	 */
	if (nblocks != 0)
	{
		stat->scanned_percent = 100 * scanned / nblocks;
		stat->tuple_percent = 100.0 * stat->tuple_len / stat->table_len;
		stat->dead_tuple_percent = 100.0 * stat->dead_tuple_len / stat->table_len;
		stat->free_percent = 100.0 * stat->free_space / stat->table_len;
	}

	if (BufferIsValid(vmbuffer))
	{
		ReleaseBuffer(vmbuffer);
		vmbuffer = InvalidBuffer;
	}
}

/*
 * Returns estimated live/dead tuple statistics for the given relid.
 *
 * The superuser() check here must be kept as the library might be upgraded
 * without the extension being upgraded, meaning that in pre-1.5 installations
 * these functions could be called by any user.
 */
Datum
pgstattuple_approx(PG_FUNCTION_ARGS)
{
	Oid			relid = PG_GETARG_OID(0);

	if (!superuser())
		ereport(ERROR,
				(errcode(ERRCODE_INSUFFICIENT_PRIVILEGE),
				 (errmsg("must be superuser to use pgstattuple functions"))));

	PG_RETURN_DATUM(pgstattuple_approx_internal(relid, fcinfo));
}

/*
 * As of pgstattuple version 1.5, we no longer need to check if the user
 * is a superuser because we REVOKE EXECUTE on the SQL function from PUBLIC.
 * Users can then grant access to it based on their policies.
 *
 * Otherwise identical to pgstattuple_approx (above).
 */
Datum
pgstattuple_approx_v1_5(PG_FUNCTION_ARGS)
{
	Oid			relid = PG_GETARG_OID(0);

	PG_RETURN_DATUM(pgstattuple_approx_internal(relid, fcinfo));
}

Datum
pgstattuple_approx_internal(Oid relid, FunctionCallInfo fcinfo)
{
	Relation	rel;
	output_type stat = {0};
	TupleDesc	tupdesc;
	bool		nulls[NUM_OUTPUT_COLUMNS];
	Datum		values[NUM_OUTPUT_COLUMNS];
	HeapTuple	ret;
	int			i = 0;

	if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
		elog(ERROR, "return type must be a row type");

	if (tupdesc->natts != NUM_OUTPUT_COLUMNS)
		elog(ERROR, "incorrect number of output arguments");

	rel = relation_open(relid, AccessShareLock);

	/*
	 * Reject attempts to read non-local temporary relations; we would be
	 * likely to get wrong data since we have no visibility into the owning
	 * session's local buffers.
	 */
	if (RELATION_IS_OTHER_TEMP(rel))
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("cannot access temporary tables of other sessions")));

	/*
	 * We support only ordinary relations and materialised views, because we
	 * depend on the visibility map and free space map for our estimates about
	 * unscanned pages.
	 */
	if (!(rel->rd_rel->relkind == RELKIND_RELATION ||
		  rel->rd_rel->relkind == RELKIND_MATVIEW))
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("\"%s\" is not a table or materialized view",
						RelationGetRelationName(rel))));

	statapprox_heap(rel, &stat);

	relation_close(rel, AccessShareLock);

	memset(nulls, 0, sizeof(nulls));

	values[i++] = Int64GetDatum(stat.table_len);
	values[i++] = Float8GetDatum(stat.scanned_percent);
	values[i++] = Int64GetDatum(stat.tuple_count);
	values[i++] = Int64GetDatum(stat.tuple_len);
	values[i++] = Float8GetDatum(stat.tuple_percent);
	values[i++] = Int64GetDatum(stat.dead_tuple_count);
	values[i++] = Int64GetDatum(stat.dead_tuple_len);
	values[i++] = Float8GetDatum(stat.dead_tuple_percent);
	values[i++] = Int64GetDatum(stat.free_space);
	values[i++] = Float8GetDatum(stat.free_percent);

	ret = heap_form_tuple(tupdesc, values, nulls);
	return HeapTupleGetDatum(ret);
}
