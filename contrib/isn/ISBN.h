/*
 * ISBN.h
 *      PostgreSQL type definitions for ISNs (ISBN, ISMN, ISSN, EAN13, UPC)
 *
 * Information recompiled by Kronuz on June 20, 2006
 * http://www.isbn-international.org/
 * http://www.isbn.org/
 *
 * IDENTIFICATION
 *      contrib/isn/ISBN.h
 *
 * 0-393-04002-X => 039304002(X) <=> 039304002 <=> (978)039304002 <=> 978039304002(9) <=> 978-0-393-04002-9
 *
 *
 * ISBN            0    3     9      3    0   4    0    0    2
 * Weight        10    9     8      7    6   5    4    3    2
 * Product        0 + 27 + 72 + 21 + 0 + 20 + 0 + 0 + 4 = 144
 *                144 / 11 = 13 remainder 1
 * Check digit    11 - 1 = 10 = X
 * => 0-393-04002-X
 *
 * ISBN            9    7     8     0     3     9      3   0   4   0   0   2
 * Weight        1    3     1     3     1     3      1   3   1   3   1   3
 * Product        9 + 21 + 8 + 0 + 3 + 27 + 3 + 0 + 4 + 0 + 0 + 6 = 81
 *                81 / 10 = 8 remainder 1
 * Check digit    10 - 1 = 9
 * => 978-0-393-04002-9
 *
 */

/*
 * For ISBN with prefix 978
 * Range Table as of 2010-Jul-29
 */

/* where the digit set begins, and how many of them are in the table */
const unsigned ISBN_index[10][2] = {
    {0, 6},
    {6, 6},
    {12, 8},
    {20, 14},
    {34, 6},
    {40, 19},
    {59, 68},
    {127, 5},
    {132, 60},
    {192, 718},
};

const char *ISBN_range[][2] = {
    {"0-00", "0-19"},
    {"0-200", "0-699"},
    {"0-7000", "0-8499"},
    {"0-85000", "0-89999"},
    {"0-900000", "0-949999"},
    {"0-9500000", "0-9999999"},
    {"1-00", "1-09"},
    {"1-100", "1-399"},
    {"1-4000", "1-5499"},
    {"1-55000", "1-86979"},
    {"1-869800", "1-998999"},
    {"1-9990000", "1-9999999"},
    {"2-00", "2-19"},
    {"2-200", "2-349"},
    {"2-35000", "2-39999"},
    {"2-400", "2-699"},
    {"2-7000", "2-8399"},
    {"2-84000", "2-89999"},
    {"2-900000", "2-949999"},
    {"2-9500000", "2-9999999"},
    {"3-00", "3-02"},
    {"3-030", "3-033"},
    {"3-0340", "3-0369"},
    {"3-03700", "3-03999"},
    {"3-04", "3-19"},
    {"3-200", "3-699"},
    {"3-7000", "3-8499"},
    {"3-85000", "3-89999"},
    {"3-900000", "3-949999"},
    {"3-9500000", "3-9539999"},
    {"3-95400", "3-96999"},
    {"3-9700000", "3-9899999"},
    {"3-99000", "3-99499"},
    {"3-99500", "3-99999"},
    {"4-00", "4-19"},
    {"4-200", "4-699"},
    {"4-7000", "4-8499"},
    {"4-85000", "4-89999"},
    {"4-900000", "4-949999"},
    {"4-9500000", "4-9999999"},
    {"5-00", "5-19"},
    {"5-200", "5-420"},
    {"5-4210", "5-4299"},
    {"5-430", "5-430"},
    {"5-4310", "5-4399"},
    {"5-440", "5-440"},
    {"5-4410", "5-4499"},
    {"5-450", "5-699"},
    {"5-7000", "5-8499"},
    {"5-85000", "5-89999"},
    {"5-900000", "5-909999"},
    {"5-91000", "5-91999"},
    {"5-9200", "5-9299"},
    {"5-93000", "5-94999"},
    {"5-9500000", "5-9500999"},
    {"5-9501", "5-9799"},
    {"5-98000", "5-98999"},
    {"5-9900000", "5-9909999"},
    {"5-9910", "5-9999"},
    {"600-00", "600-09"},
    {"600-100", "600-499"},
    {"600-5000", "600-8999"},
    {"600-90000", "600-99999"},
    {"601-00", "601-19"},
    {"601-200", "601-699"},
    {"601-7000", "601-7999"},
    {"601-80000", "601-84999"},
    {"601-85", "601-99"},
    {"602-00", "602-19"},
    {"602-200", "602-799"},
    {"602-8000", "602-9499"},
    {"602-95000", "602-99999"},
    {"603-00", "603-04"},
    {"603-05", "603-49"},
    {"603-500", "603-799"},
    {"603-8000", "603-8999"},
    {"603-90000", "603-99999"},
    {"604-0", "604-4"},
    {"604-50", "604-89"},
    {"604-900", "604-979"},
    {"604-9800", "604-9999"},
    {"605-01", "605-09"},
    {"605-100", "605-399"},
    {"605-4000", "605-5999"},
    {"605-60000", "605-89999"},
    {"605-90", "605-99"},
    {"606-0", "606-0"},
    {"606-10", "606-49"},
    {"606-500", "606-799"},
    {"606-8000", "606-9199"},
    {"606-92000", "606-99999"},
    {"607-00", "607-39"},
    {"607-400", "607-749"},
    {"607-7500", "607-9499"},
    {"607-95000", "607-99999"},
    {"608-0", "608-0"},
    {"608-10", "608-19"},
    {"608-200", "608-449"},
    {"608-4500", "608-6499"},
    {"608-65000", "608-69999"},
    {"608-7", "608-9"},
    {"609-00", "609-39"},
    {"609-400", "609-799"},
    {"609-8000", "609-9499"},
    {"609-95000", "609-99999"},
    {"612-00", "612-29"},
    {"612-300", "612-399"},
    {"612-4000", "612-4499"},
    {"612-45000", "612-49999"},
    {"612-50", "612-99"},
    {"613-0", "613-9"},
    {"614-00", "614-39"},
    {"614-400", "614-799"},
    {"614-8000", "614-9499"},
    {"614-95000", "614-99999"},
    {"615-00", "615-09"},
    {"615-100", "615-499"},
    {"615-5000", "615-7999"},
    {"615-80000", "615-89999"},
    {"616-00", "616-19"},
    {"616-200", "616-699"},
    {"616-7000", "616-8999"},
    {"616-90000", "616-99999"},
    {"617-00", "617-49"},
    {"617-500", "617-699"},
    {"617-7000", "617-8999"},
    {"617-90000", "617-99999"},
    {"7-00", "7-09"},
    {"7-100", "7-499"},
    {"7-5000", "7-7999"},
    {"7-80000", "7-89999"},
    {"7-900000", "7-999999"},
    {"80-00", "80-19"},
    {"80-200", "80-699"},
    {"80-7000", "80-8499"},
    {"80-85000", "80-89999"},
    {"80-900000", "80-999999"},
    {"81-00", "81-19"},
    {"81-200", "81-699"},
    {"81-7000", "81-8499"},
    {"81-85000", "81-89999"},
    {"81-900000", "81-999999"},
    {"82-00", "82-19"},
    {"82-200", "82-699"},
    {"82-7000", "82-8999"},
    {"82-90000", "82-98999"},
    {"82-990000", "82-999999"},
    {"83-00", "83-19"},
    {"83-200", "83-599"},
    {"83-60000", "83-69999"},
    {"83-7000", "83-8499"},
    {"83-85000", "83-89999"},
    {"83-900000", "83-999999"},
    {"84-00", "84-14"},
    {"84-15000", "84-19999"},
    {"84-200", "84-699"},
    {"84-7000", "84-8499"},
    {"84-85000", "84-89999"},
    {"84-9000", "84-9199"},
    {"84-920000", "84-923999"},
    {"84-92400", "84-92999"},
    {"84-930000", "84-949999"},
    {"84-95000", "84-96999"},
    {"84-9700", "84-9999"},
    {"85-00", "85-19"},
    {"85-200", "85-599"},
    {"85-60000", "85-69999"},
    {"85-7000", "85-8499"},
    {"85-85000", "85-89999"},
    {"85-900000", "85-979999"},
    {"85-98000", "85-99999"},
    {"86-00", "86-29"},
    {"86-300", "86-599"},
    {"86-6000", "86-7999"},
    {"86-80000", "86-89999"},
    {"86-900000", "86-999999"},
    {"87-00", "87-29"},
    {"87-400", "87-649"},
    {"87-7000", "87-7999"},
    {"87-85000", "87-94999"},
    {"87-970000", "87-999999"},
    {"88-00", "88-19"},
    {"88-200", "88-599"},
    {"88-6000", "88-8499"},
    {"88-85000", "88-89999"},
    {"88-900000", "88-949999"},
    {"88-95000", "88-99999"},
    {"89-00", "89-24"},
    {"89-250", "89-549"},
    {"89-5500", "89-8499"},
    {"89-85000", "89-94999"},
    {"89-950000", "89-999999"},
    {"90-00", "90-19"},
    {"90-200", "90-499"},
    {"90-5000", "90-6999"},
    {"90-70000", "90-79999"},
    {"90-800000", "90-849999"},
    {"90-8500", "90-8999"},
    {"90-90", "90-90"},
    {"90-910000", "90-939999"},
    {"90-94", "90-94"},
    {"90-950000", "90-999999"},
    {"91-0", "91-1"},
    {"91-20", "91-49"},
    {"91-500", "91-649"},
    {"91-7000", "91-7999"},
    {"91-85000", "91-94999"},
    {"91-970000", "91-999999"},
    {"92-0", "92-5"},
    {"92-60", "92-79"},
    {"92-800", "92-899"},
    {"92-9000", "92-9499"},
    {"92-95000", "92-98999"},
    {"92-990000", "92-999999"},
    {"93-00", "93-09"},
    {"93-100", "93-499"},
    {"93-5000", "93-7999"},
    {"93-80000", "93-94999"},
    {"93-950000", "93-999999"},
    {"94-000", "94-599"},
    {"94-6000", "94-8999"},
    {"94-90000", "94-99999"},
    {"950-00", "950-49"},
    {"950-500", "950-899"},
    {"950-9000", "950-9899"},
    {"950-99000", "950-99999"},
    {"951-0", "951-1"},
    {"951-20", "951-54"},
    {"951-550", "951-889"},
    {"951-8900", "951-9499"},
    {"951-95000", "951-99999"},
    {"952-00", "952-19"},
    {"952-200", "952-499"},
    {"952-5000", "952-5999"},
    {"952-60", "952-65"},
    {"952-6600", "952-6699"},
    {"952-67000", "952-69999"},
    {"952-7000", "952-7999"},
    {"952-80", "952-94"},
    {"952-9500", "952-9899"},
    {"952-99000", "952-99999"},
    {"953-0", "953-0"},
    {"953-10", "953-14"},
    {"953-150", "953-549"},
    {"953-55000", "953-59999"},
    {"953-6000", "953-9499"},
    {"953-95000", "953-99999"},
    {"954-00", "954-28"},
    {"954-2900", "954-2999"},
    {"954-300", "954-799"},
    {"954-8000", "954-8999"},
    {"954-90000", "954-92999"},
    {"954-9300", "954-9999"},
    {"955-0000", "955-1999"},
    {"955-20", "955-49"},
    {"955-50000", "955-54999"},
    {"955-550", "955-799"},
    {"955-8000", "955-9499"},
    {"955-95000", "955-99999"},
    {"956-00", "956-19"},
    {"956-200", "956-699"},
    {"956-7000", "956-9999"},
    {"957-00", "957-02"},
    {"957-0300", "957-0499"},
    {"957-05", "957-19"},
    {"957-2000", "957-2099"},
    {"957-21", "957-27"},
    {"957-28000", "957-30999"},
    {"957-31", "957-43"},
    {"957-440", "957-819"},
    {"957-8200", "957-9699"},
    {"957-97000", "957-99999"},
    {"958-00", "958-56"},
    {"958-57000", "958-59999"},
    {"958-600", "958-799"},
    {"958-8000", "958-9499"},
    {"958-95000", "958-99999"},
    {"959-00", "959-19"},
    {"959-200", "959-699"},
    {"959-7000", "959-8499"},
    {"959-85000", "959-99999"},
    {"960-00", "960-19"},
    {"960-200", "960-659"},
    {"960-6600", "960-6899"},
    {"960-690", "960-699"},
    {"960-7000", "960-8499"},
    {"960-85000", "960-92999"},
    {"960-93", "960-93"},
    {"960-9400", "960-9799"},
    {"960-98000", "960-99999"},
    {"961-00", "961-19"},
    {"961-200", "961-599"},
    {"961-6000", "961-8999"},
    {"961-90000", "961-94999"},
    {"962-00", "962-19"},
    {"962-200", "962-699"},
    {"962-7000", "962-8499"},
    {"962-85000", "962-86999"},
    {"962-8700", "962-8999"},
    {"962-900", "962-999"},
    {"963-00", "963-19"},
    {"963-200", "963-699"},
    {"963-7000", "963-8499"},
    {"963-85000", "963-89999"},
    {"963-9000", "963-9999"},
    {"964-00", "964-14"},
    {"964-150", "964-249"},
    {"964-2500", "964-2999"},
    {"964-300", "964-549"},
    {"964-5500", "964-8999"},
    {"964-90000", "964-96999"},
    {"964-970", "964-989"},
    {"964-9900", "964-9999"},
    {"965-00", "965-19"},
    {"965-200", "965-599"},
    {"965-7000", "965-7999"},
    {"965-90000", "965-99999"},
    {"966-00", "966-14"},
    {"966-1500", "966-1699"},
    {"966-170", "966-199"},
    {"966-2000", "966-2999"},
    {"966-300", "966-699"},
    {"966-7000", "966-8999"},
    {"966-90000", "966-99999"},
    {"967-00", "967-29"},
    {"967-300", "967-499"},
    {"967-5000", "967-5999"},
    {"967-60", "967-89"},
    {"967-900", "967-989"},
    {"967-9900", "967-9989"},
    {"967-99900", "967-99999"},
    {"968-01", "968-39"},
    {"968-400", "968-499"},
    {"968-5000", "968-7999"},
    {"968-800", "968-899"},
    {"968-9000", "968-9999"},
    {"969-0", "969-1"},
    {"969-20", "969-39"},
    {"969-400", "969-799"},
    {"969-8000", "969-9999"},
    {"970-01", "970-59"},
    {"970-600", "970-899"},
    {"970-9000", "970-9099"},
    {"970-91000", "970-96999"},
    {"970-9700", "970-9999"},
    {"971-000", "971-015"},
    {"971-0160", "971-0199"},
    {"971-02", "971-02"},
    {"971-0300", "971-0599"},
    {"971-06", "971-09"},
    {"971-10", "971-49"},
    {"971-500", "971-849"},
    {"971-8500", "971-9099"},
    {"971-91000", "971-98999"},
    {"971-9900", "971-9999"},
    {"972-0", "972-1"},
    {"972-20", "972-54"},
    {"972-550", "972-799"},
    {"972-8000", "972-9499"},
    {"972-95000", "972-99999"},
    {"973-0", "973-0"},
    {"973-100", "973-169"},
    {"973-1700", "973-1999"},
    {"973-20", "973-54"},
    {"973-550", "973-759"},
    {"973-7600", "973-8499"},
    {"973-85000", "973-88999"},
    {"973-8900", "973-9499"},
    {"973-95000", "973-99999"},
    {"974-00", "974-19"},
    {"974-200", "974-699"},
    {"974-7000", "974-8499"},
    {"974-85000", "974-89999"},
    {"974-90000", "974-94999"},
    {"974-9500", "974-9999"},
    {"975-00000", "975-00999"},
    {"975-01", "975-01"},
    {"975-02", "975-24"},
    {"975-250", "975-599"},
    {"975-6000", "975-9199"},
    {"975-92000", "975-98999"},
    {"975-990", "975-999"},
    {"976-0", "976-3"},
    {"976-40", "976-59"},
    {"976-600", "976-799"},
    {"976-8000", "976-9499"},
    {"976-95000", "976-99999"},
    {"977-00", "977-19"},
    {"977-200", "977-499"},
    {"977-5000", "977-6999"},
    {"977-700", "977-999"},
    {"978-000", "978-199"},
    {"978-2000", "978-2999"},
    {"978-30000", "978-79999"},
    {"978-8000", "978-8999"},
    {"978-900", "978-999"},
    {"979-000", "979-099"},
    {"979-1000", "979-1499"},
    {"979-15000", "979-19999"},
    {"979-20", "979-29"},
    {"979-3000", "979-3999"},
    {"979-400", "979-799"},
    {"979-8000", "979-9499"},
    {"979-95000", "979-99999"},
    {"980-00", "980-19"},
    {"980-200", "980-599"},
    {"980-6000", "980-9999"},
    {"981-00", "981-11"},
    {"981-1200", "981-1999"},
    {"981-200", "981-289"},
    {"981-2900", "981-9999"},
    {"982-00", "982-09"},
    {"982-100", "982-699"},
    {"982-70", "982-89"},
    {"982-9000", "982-9799"},
    {"982-98000", "982-99999"},
    {"983-00", "983-01"},
    {"983-020", "983-199"},
    {"983-2000", "983-3999"},
    {"983-40000", "983-44999"},
    {"983-45", "983-49"},
    {"983-50", "983-79"},
    {"983-800", "983-899"},
    {"983-9000", "983-9899"},
    {"983-99000", "983-99999"},
    {"984-00", "984-39"},
    {"984-400", "984-799"},
    {"984-8000", "984-8999"},
    {"984-90000", "984-99999"},
    {"985-00", "985-39"},
    {"985-400", "985-599"},
    {"985-6000", "985-8999"},
    {"985-90000", "985-99999"},
    {"986-00", "986-11"},
    {"986-120", "986-559"},
    {"986-5600", "986-7999"},
    {"986-80000", "986-99999"},
    {"987-00", "987-09"},
    {"987-1000", "987-1999"},
    {"987-20000", "987-29999"},
    {"987-30", "987-49"},
    {"987-500", "987-899"},
    {"987-9000", "987-9499"},
    {"987-95000", "987-99999"},
    {"988-00", "988-16"},
    {"988-17000", "988-19999"},
    {"988-200", "988-799"},
    {"988-8000", "988-9699"},
    {"988-97000", "988-99999"},
    {"989-0", "989-1"},
    {"989-20", "989-54"},
    {"989-550", "989-799"},
    {"989-8000", "989-9499"},
    {"989-95000", "989-99999"},
    {"9927-00", "9927-09"},
    {"9927-100", "9927-399"},
    {"9927-4000", "9927-4999"},
    {"9928-00", "9928-09"},
    {"9928-100", "9928-399"},
    {"9928-4000", "9928-4999"},
    {"9929-0", "9929-3"},
    {"9929-40", "9929-54"},
    {"9929-550", "9929-799"},
    {"9929-8000", "9929-9999"},
    {"9930-00", "9930-49"},
    {"9930-500", "9930-939"},
    {"9930-9400", "9930-9999"},
    {"9931-00", "9931-29"},
    {"9931-300", "9931-899"},
    {"9931-9000", "9931-9999"},
    {"9932-00", "9932-39"},
    {"9932-400", "9932-849"},
    {"9932-8500", "9932-9999"},
    {"9933-0", "9933-0"},
    {"9933-10", "9933-39"},
    {"9933-400", "9933-899"},
    {"9933-9000", "9933-9999"},
    {"9934-0", "9934-0"},
    {"9934-10", "9934-49"},
    {"9934-500", "9934-799"},
    {"9934-8000", "9934-9999"},
    {"9935-0", "9935-0"},
    {"9935-10", "9935-39"},
    {"9935-400", "9935-899"},
    {"9935-9000", "9935-9999"},
    {"9936-0", "9936-1"},
    {"9936-20", "9936-39"},
    {"9936-400", "9936-799"},
    {"9936-8000", "9936-9999"},
    {"9937-0", "9937-2"},
    {"9937-30", "9937-49"},
    {"9937-500", "9937-799"},
    {"9937-8000", "9937-9999"},
    {"9938-00", "9938-79"},
    {"9938-800", "9938-949"},
    {"9938-9500", "9938-9999"},
    {"9939-0", "9939-4"},
    {"9939-50", "9939-79"},
    {"9939-800", "9939-899"},
    {"9939-9000", "9939-9999"},
    {"9940-0", "9940-1"},
    {"9940-20", "9940-49"},
    {"9940-500", "9940-899"},
    {"9940-9000", "9940-9999"},
    {"9941-0", "9941-0"},
    {"9941-10", "9941-39"},
    {"9941-400", "9941-899"},
    {"9941-9000", "9941-9999"},
    {"9942-00", "9942-89"},
    {"9942-900", "9942-994"},
    {"9942-9950", "9942-9999"},
    {"9943-00", "9943-29"},
    {"9943-300", "9943-399"},
    {"9943-4000", "9943-9999"},
    {"9944-0000", "9944-0999"},
    {"9944-100", "9944-499"},
    {"9944-5000", "9944-5999"},
    {"9944-60", "9944-69"},
    {"9944-700", "9944-799"},
    {"9944-80", "9944-89"},
    {"9944-900", "9944-999"},
    {"9945-00", "9945-00"},
    {"9945-010", "9945-079"},
    {"9945-08", "9945-39"},
    {"9945-400", "9945-569"},
    {"9945-57", "9945-57"},
    {"9945-580", "9945-849"},
    {"9945-8500", "9945-9999"},
    {"9946-0", "9946-1"},
    {"9946-20", "9946-39"},
    {"9946-400", "9946-899"},
    {"9946-9000", "9946-9999"},
    {"9947-0", "9947-1"},
    {"9947-20", "9947-79"},
    {"9947-800", "9947-999"},
    {"9948-00", "9948-39"},
    {"9948-400", "9948-849"},
    {"9948-8500", "9948-9999"},
    {"9949-0", "9949-0"},
    {"9949-10", "9949-39"},
    {"9949-400", "9949-899"},
    {"9949-9000", "9949-9999"},
    {"9950-00", "9950-29"},
    {"9950-300", "9950-849"},
    {"9950-8500", "9950-9999"},
    {"9951-00", "9951-39"},
    {"9951-400", "9951-849"},
    {"9951-8500", "9951-9999"},
    {"9952-0", "9952-1"},
    {"9952-20", "9952-39"},
    {"9952-400", "9952-799"},
    {"9952-8000", "9952-9999"},
    {"9953-0", "9953-0"},
    {"9953-10", "9953-39"},
    {"9953-400", "9953-599"},
    {"9953-60", "9953-89"},
    {"9953-9000", "9953-9999"},
    {"9954-0", "9954-1"},
    {"9954-20", "9954-39"},
    {"9954-400", "9954-799"},
    {"9954-8000", "9954-9999"},
    {"9955-00", "9955-39"},
    {"9955-400", "9955-929"},
    {"9955-9300", "9955-9999"},
    {"9956-0", "9956-0"},
    {"9956-10", "9956-39"},
    {"9956-400", "9956-899"},
    {"9956-9000", "9956-9999"},
    {"9957-00", "9957-39"},
    {"9957-400", "9957-699"},
    {"9957-70", "9957-84"},
    {"9957-8500", "9957-8799"},
    {"9957-88", "9957-99"},
    {"9958-0", "9958-0"},
    {"9958-10", "9958-49"},
    {"9958-500", "9958-899"},
    {"9958-9000", "9958-9999"},
    {"9959-0", "9959-1"},
    {"9959-20", "9959-79"},
    {"9959-800", "9959-949"},
    {"9959-9500", "9959-9999"},
    {"9960-00", "9960-59"},
    {"9960-600", "9960-899"},
    {"9960-9000", "9960-9999"},
    {"9961-0", "9961-2"},
    {"9961-30", "9961-69"},
    {"9961-700", "9961-949"},
    {"9961-9500", "9961-9999"},
    {"9962-00", "9962-54"},
    {"9962-5500", "9962-5599"},
    {"9962-56", "9962-59"},
    {"9962-600", "9962-849"},
    {"9962-8500", "9962-9999"},
    {"9963-0", "9963-2"},
    {"9963-30", "9963-54"},
    {"9963-550", "9963-734"},
    {"9963-7350", "9963-7499"},
    {"9963-7500", "9963-9999"},
    {"9964-0", "9964-6"},
    {"9964-70", "9964-94"},
    {"9964-950", "9964-999"},
    {"9965-00", "9965-39"},
    {"9965-400", "9965-899"},
    {"9965-9000", "9965-9999"},
    {"9966-000", "9966-199"},
    {"9966-20", "9966-69"},
    {"9966-7000", "9966-7499"},
    {"9966-750", "9966-959"},
    {"9966-9600", "9966-9999"},
    {"9967-00", "9967-39"},
    {"9967-400", "9967-899"},
    {"9967-9000", "9967-9999"},
    {"9968-00", "9968-49"},
    {"9968-500", "9968-939"},
    {"9968-9400", "9968-9999"},
    {"9970-00", "9970-39"},
    {"9970-400", "9970-899"},
    {"9970-9000", "9970-9999"},
    {"9971-0", "9971-5"},
    {"9971-60", "9971-89"},
    {"9971-900", "9971-989"},
    {"9971-9900", "9971-9999"},
    {"9972-00", "9972-09"},
    {"9972-1", "9972-1"},
    {"9972-200", "9972-249"},
    {"9972-2500", "9972-2999"},
    {"9972-30", "9972-59"},
    {"9972-600", "9972-899"},
    {"9972-9000", "9972-9999"},
    {"9973-00", "9973-05"},
    {"9973-060", "9973-089"},
    {"9973-0900", "9973-0999"},
    {"9973-10", "9973-69"},
    {"9973-700", "9973-969"},
    {"9973-9700", "9973-9999"},
    {"9974-0", "9974-2"},
    {"9974-30", "9974-54"},
    {"9974-550", "9974-749"},
    {"9974-7500", "9974-9499"},
    {"9974-95", "9974-99"},
    {"9975-0", "9975-0"},
    {"9975-100", "9975-399"},
    {"9975-4000", "9975-4499"},
    {"9975-45", "9975-89"},
    {"9975-900", "9975-949"},
    {"9975-9500", "9975-9999"},
    {"9976-0", "9976-5"},
    {"9976-60", "9976-89"},
    {"9976-900", "9976-989"},
    {"9976-9900", "9976-9999"},
    {"9977-00", "9977-89"},
    {"9977-900", "9977-989"},
    {"9977-9900", "9977-9999"},
    {"9978-00", "9978-29"},
    {"9978-300", "9978-399"},
    {"9978-40", "9978-94"},
    {"9978-950", "9978-989"},
    {"9978-9900", "9978-9999"},
    {"9979-0", "9979-4"},
    {"9979-50", "9979-64"},
    {"9979-650", "9979-659"},
    {"9979-66", "9979-75"},
    {"9979-760", "9979-899"},
    {"9979-9000", "9979-9999"},
    {"9980-0", "9980-3"},
    {"9980-40", "9980-89"},
    {"9980-900", "9980-989"},
    {"9980-9900", "9980-9999"},
    {"9981-00", "9981-09"},
    {"9981-100", "9981-159"},
    {"9981-1600", "9981-1999"},
    {"9981-20", "9981-79"},
    {"9981-800", "9981-949"},
    {"9981-9500", "9981-9999"},
    {"9982-00", "9982-79"},
    {"9982-800", "9982-989"},
    {"9982-9900", "9982-9999"},
    {"9983-80", "9983-94"},
    {"9983-950", "9983-989"},
    {"9983-9900", "9983-9999"},
    {"9984-00", "9984-49"},
    {"9984-500", "9984-899"},
    {"9984-9000", "9984-9999"},
    {"9985-0", "9985-4"},
    {"9985-50", "9985-79"},
    {"9985-800", "9985-899"},
    {"9985-9000", "9985-9999"},
    {"9986-00", "9986-39"},
    {"9986-400", "9986-899"},
    {"9986-9000", "9986-9399"},
    {"9986-940", "9986-969"},
    {"9986-97", "9986-99"},
    {"9987-00", "9987-39"},
    {"9987-400", "9987-879"},
    {"9987-8800", "9987-9999"},
    {"9988-0", "9988-2"},
    {"9988-30", "9988-54"},
    {"9988-550", "9988-749"},
    {"9988-7500", "9988-9999"},
    {"9989-0", "9989-0"},
    {"9989-100", "9989-199"},
    {"9989-2000", "9989-2999"},
    {"9989-30", "9989-59"},
    {"9989-600", "9989-949"},
    {"9989-9500", "9989-9999"},
    {"99901-00", "99901-49"},
    {"99901-500", "99901-799"},
    {"99901-80", "99901-99"},
    {"99903-0", "99903-1"},
    {"99903-20", "99903-89"},
    {"99903-900", "99903-999"},
    {"99904-0", "99904-5"},
    {"99904-60", "99904-89"},
    {"99904-900", "99904-999"},
    {"99905-0", "99905-3"},
    {"99905-40", "99905-79"},
    {"99905-800", "99905-999"},
    {"99906-0", "99906-2"},
    {"99906-30", "99906-59"},
    {"99906-600", "99906-699"},
    {"99906-70", "99906-89"},
    {"99906-90", "99906-94"},
    {"99906-950", "99906-999"},
    {"99908-0", "99908-0"},
    {"99908-10", "99908-89"},
    {"99908-900", "99908-999"},
    {"99909-0", "99909-3"},
    {"99909-40", "99909-94"},
    {"99909-950", "99909-999"},
    {"99910-0", "99910-2"},
    {"99910-30", "99910-89"},
    {"99910-900", "99910-999"},
    {"99911-00", "99911-59"},
    {"99911-600", "99911-999"},
    {"99912-0", "99912-3"},
    {"99912-400", "99912-599"},
    {"99912-60", "99912-89"},
    {"99912-900", "99912-999"},
    {"99913-0", "99913-2"},
    {"99913-30", "99913-35"},
    {"99913-600", "99913-604"},
    {"99914-0", "99914-4"},
    {"99914-50", "99914-89"},
    {"99914-900", "99914-999"},
    {"99915-0", "99915-4"},
    {"99915-50", "99915-79"},
    {"99915-800", "99915-999"},
    {"99916-0", "99916-2"},
    {"99916-30", "99916-69"},
    {"99916-700", "99916-999"},
    {"99917-0", "99917-2"},
    {"99917-30", "99917-89"},
    {"99917-900", "99917-999"},
    {"99918-0", "99918-3"},
    {"99918-40", "99918-79"},
    {"99918-800", "99918-999"},
    {"99919-0", "99919-2"},
    {"99919-300", "99919-399"},
    {"99919-40", "99919-69"},
    {"99919-900", "99919-999"},
    {"99920-0", "99920-4"},
    {"99920-50", "99920-89"},
    {"99920-900", "99920-999"},
    {"99921-0", "99921-1"},
    {"99921-20", "99921-69"},
    {"99921-700", "99921-799"},
    {"99921-8", "99921-8"},
    {"99921-90", "99921-99"},
    {"99922-0", "99922-3"},
    {"99922-40", "99922-69"},
    {"99922-700", "99922-999"},
    {"99923-0", "99923-1"},
    {"99923-20", "99923-79"},
    {"99923-800", "99923-999"},
    {"99924-0", "99924-1"},
    {"99924-20", "99924-79"},
    {"99924-800", "99924-999"},
    {"99925-0", "99925-3"},
    {"99925-40", "99925-79"},
    {"99925-800", "99925-999"},
    {"99926-0", "99926-0"},
    {"99926-10", "99926-59"},
    {"99926-600", "99926-999"},
    {"99927-0", "99927-2"},
    {"99927-30", "99927-59"},
    {"99927-600", "99927-999"},
    {"99928-0", "99928-0"},
    {"99928-10", "99928-79"},
    {"99928-800", "99928-999"},
    {"99929-0", "99929-4"},
    {"99929-50", "99929-79"},
    {"99929-800", "99929-999"},
    {"99930-0", "99930-4"},
    {"99930-50", "99930-79"},
    {"99930-800", "99930-999"},
    {"99931-0", "99931-4"},
    {"99931-50", "99931-79"},
    {"99931-800", "99931-999"},
    {"99932-0", "99932-0"},
    {"99932-10", "99932-59"},
    {"99932-600", "99932-699"},
    {"99932-7", "99932-7"},
    {"99932-80", "99932-99"},
    {"99933-0", "99933-2"},
    {"99933-30", "99933-59"},
    {"99933-600", "99933-999"},
    {"99934-0", "99934-1"},
    {"99934-20", "99934-79"},
    {"99934-800", "99934-999"},
    {"99935-0", "99935-2"},
    {"99935-30", "99935-59"},
    {"99935-600", "99935-699"},
    {"99935-7", "99935-8"},
    {"99935-90", "99935-99"},
    {"99936-0", "99936-0"},
    {"99936-10", "99936-59"},
    {"99936-600", "99936-999"},
    {"99937-0", "99937-1"},
    {"99937-20", "99937-59"},
    {"99937-600", "99937-999"},
    {"99938-0", "99938-1"},
    {"99938-20", "99938-59"},
    {"99938-600", "99938-899"},
    {"99938-90", "99938-99"},
    {"99939-0", "99939-5"},
    {"99939-60", "99939-89"},
    {"99939-900", "99939-999"},
    {"99940-0", "99940-0"},
    {"99940-10", "99940-69"},
    {"99940-700", "99940-999"},
    {"99941-0", "99941-2"},
    {"99941-30", "99941-79"},
    {"99941-800", "99941-999"},
    {"99942-0", "99942-4"},
    {"99942-50", "99942-79"},
    {"99942-800", "99942-999"},
    {"99943-0", "99943-2"},
    {"99943-30", "99943-59"},
    {"99943-600", "99943-999"},
    {"99944-0", "99944-4"},
    {"99944-50", "99944-79"},
    {"99944-800", "99944-999"},
    {"99945-0", "99945-5"},
    {"99945-60", "99945-89"},
    {"99945-900", "99945-999"},
    {"99946-0", "99946-2"},
    {"99946-30", "99946-59"},
    {"99946-600", "99946-999"},
    {"99947-0", "99947-2"},
    {"99947-30", "99947-69"},
    {"99947-700", "99947-999"},
    {"99948-0", "99948-4"},
    {"99948-50", "99948-79"},
    {"99948-800", "99948-999"},
    {"99949-0", "99949-1"},
    {"99949-20", "99949-89"},
    {"99949-900", "99949-999"},
    {"99950-0", "99950-4"},
    {"99950-50", "99950-79"},
    {"99950-800", "99950-999"},
    {"99952-0", "99952-4"},
    {"99952-50", "99952-79"},
    {"99952-800", "99952-999"},
    {"99953-0", "99953-2"},
    {"99953-30", "99953-79"},
    {"99953-800", "99953-939"},
    {"99953-94", "99953-99"},
    {"99954-0", "99954-2"},
    {"99954-30", "99954-69"},
    {"99954-700", "99954-999"},
    {"99955-0", "99955-1"},
    {"99955-20", "99955-59"},
    {"99955-600", "99955-799"},
    {"99955-80", "99955-89"},
    {"99955-90", "99955-99"},
    {"99956-00", "99956-59"},
    {"99956-600", "99956-859"},
    {"99956-86", "99956-99"},
    {"99957-0", "99957-1"},
    {"99957-20", "99957-79"},
    {"99957-800", "99957-999"},
    {"99958-0", "99958-4"},
    {"99958-50", "99958-94"},
    {"99958-950", "99958-999"},
    {"99959-0", "99959-2"},
    {"99959-30", "99959-59"},
    {"99959-600", "99959-999"},
    {"99960-0", "99960-0"},
    {"99960-10", "99960-94"},
    {"99960-950", "99960-999"},
    {"99961-0", "99961-3"},
    {"99961-40", "99961-89"},
    {"99961-900", "99961-999"},
    {"99962-0", "99962-4"},
    {"99962-50", "99962-79"},
    {"99962-800", "99962-999"},
    {"99963-00", "99963-49"},
    {"99963-500", "99963-999"},
    {"99964-0", "99964-1"},
    {"99964-20", "99964-79"},
    {"99964-800", "99964-999"},
    {"99965-0", "99965-3"},
    {"99965-40", "99965-79"},
    {"99965-800", "99965-999"},
    {"99966-0", "99966-2"},
    {"99966-30", "99966-69"},
    {"99966-700", "99966-799"},
    {"99967-0", "99967-1"},
    {"99967-20", "99967-59"},
    {"99967-600", "99967-899"},
    {NULL, NULL},
};

/*
 * For ISBN with prefix 979
 * Range Table as of 2010-Jul-29
 */

/* where the digit set begins, and how many of them are in the table */
const unsigned ISBN_index_new[10][2] = {
    {0, 0},
    {0, 5},
    {5, 0},
    {5, 0},
    {5, 0},
    {5, 0},
    {5, 0},
    {5, 0},
    {5, 0},
    {5, 0},
};

const char *ISBN_range_new[][2] = {
    {"10-00", "10-19"},
    {"10-200", "10-699"},
    {"10-7000", "10-8999"},
    {"10-90000", "10-97599"},
    {"10-976000", "10-999999"},
    {NULL, NULL},
};
