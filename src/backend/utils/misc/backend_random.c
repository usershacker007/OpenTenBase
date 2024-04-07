/*-------------------------------------------------------------------------
 *
 * backend_random.c
 *      Backend random number generation routine.
 *
 * pg_backend_random() function fills a buffer with random bytes. Normally,
 * it is just a thin wrapper around pg_strong_random(), but when compiled
 * with --disable-strong-random, we provide a built-in implementation.
 *
 * This function is used for generating nonces in authentication, and for
 * random salt generation in pgcrypto. The built-in implementation is not
 * cryptographically strong, but if the user asked for it, we'll go ahead
 * and use it anyway.
 *
 * The built-in implementation uses the standard erand48 algorithm, with
 * a seed shared between all backends.
 *
 * Portions Copyright (c) 1996-2017, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *      src/backend/utils/misc/backend_random.c
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <sys/time.h>

#include "miscadmin.h"
#include "storage/lwlock.h"
#include "storage/shmem.h"
#include "utils/backend_random.h"
#include "utils/timestamp.h"

#ifdef HAVE_STRONG_RANDOM

Size
BackendRandomShmemSize(void)
{
    return 0;
}

void
BackendRandomShmemInit(void)
{
    /* do nothing */
}

bool
pg_backend_random(char *dst, int len)
{
    /* should not be called in postmaster */
    Assert(IsUnderPostmaster || !IsPostmasterEnvironment);

    return pg_strong_random(dst, len);
}

#else

/*
 * Seed for the PRNG, stored in shared memory.
 *
 * Protected by BackendRandomLock.
 */
typedef struct
{
    bool        initialized;
    unsigned short seed[3];
}            BackendRandomShmemStruct;

static BackendRandomShmemStruct * BackendRandomShmem;

Size
BackendRandomShmemSize(void)
{
    return sizeof(BackendRandomShmemStruct);
}

void
BackendRandomShmemInit(void)
{
    bool        found;

    BackendRandomShmem = (BackendRandomShmemStruct *)
        ShmemInitStruct("Backend PRNG state",
                        BackendRandomShmemSize(),
                        &found);

    if (!IsUnderPostmaster)
    {
        Assert(!found);

        BackendRandomShmem->initialized = false;
    }
    else
        Assert(found);
}

bool
pg_backend_random(char *dst, int len)
{
    int            i;
    char       *end = dst + len;

    /* should not be called in postmaster */
    Assert(IsUnderPostmaster || !IsPostmasterEnvironment);

    LWLockAcquire(BackendRandomLock, LW_EXCLUSIVE);

    /*
     * Seed the PRNG on the first use.
     */
    if (!BackendRandomShmem->initialized)
    {
        struct timeval now;

        gettimeofday(&now, NULL);

        BackendRandomShmem->seed[0] = now.tv_sec;
        BackendRandomShmem->seed[1] = (unsigned short) (now.tv_usec);
        BackendRandomShmem->seed[2] = (unsigned short) (now.tv_usec >> 16);

        /*
         * Mix in the cancel key, generated by the postmaster. This adds what
         * little entropy the postmaster had to the seed.
         */
        BackendRandomShmem->seed[0] ^= (MyCancelKey);
        BackendRandomShmem->seed[1] ^= (MyCancelKey >> 16);

        BackendRandomShmem->initialized = true;
    }

    for (i = 0; dst < end; i++)
    {
        uint32        r;
        int            j;

        /*
         * pg_jrand48 returns a 32-bit integer. Fill the next 4 bytes from it.
         */
        r = (uint32) pg_jrand48(BackendRandomShmem->seed);

        for (j = 0; j < 4 && dst < end; j++)
        {
            *(dst++) = (char) (r & 0xFF);
            r >>= 8;
        }
    }
    LWLockRelease(BackendRandomLock);

    return true;
}


#endif                            /* HAVE_STRONG_RANDOM */
