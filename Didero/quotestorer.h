#ifndef _QUOTE_FETCHER_H
#define _QUOTE_FETCHER_H

#include "aliases.h"
#include "dbinfo.h"
#include "insertionquery.h"
#include "quote.h"

#define DB_PORT 3306
#define TABLE_NAME "deleteme"

pplx::task<void> storeQuote(MYSQL *connection, const crs_string &forexUrl, const crs_string &forexApiKey);

#endif