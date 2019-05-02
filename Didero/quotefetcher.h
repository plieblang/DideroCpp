#ifndef _QUOTE_FETCHER_H
#define _QUOTE_FETCHER_H

#include "aliases.h"
#include "dbdata.h"
#include "dbinfo.h"
#include "quote.h"

#define DB_PORT 3306
#define TABLE_NAME "deleteme"

pplx::task<void> storeFromQuoteAfterDelay(MYSQL *connection, const web::uri &url, DbData &dbData, int milliDelay);

#endif