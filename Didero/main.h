#ifndef _MAIN_H
#define _MAIN_H

#include "aliases.h"
#include "dbdata.h"
#include "dbinfo.h"
#include "deletionquery.h"
#include "insertionquery.h"
#include "quotefetcher.h"

#define DB_PORT 3306

#define WRITE_INTERVAL 180000
#define FETCH_INTERVAL 180000
#define FETCHES_PER_WRITE WRITE_INTERVAL / FETCH_INTERVAL

#define CRED_NAME L"DideroBackend"

#endif