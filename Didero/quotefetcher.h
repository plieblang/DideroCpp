#ifndef _QUOTE_FETCHER_H
#define _QUOTE_FETCHER_H

#include "aliases.h"
#include "dbinfo.h"
#include "insertquery.h"
#include "quote.h"

#define DB_PORT 3306
#define TABLE_NAME "deleteme"

pplx::task<void> getQuotes(std::queue<Quote> &quotes, const crs_string forexUrl, const crs_string forexApiKey);

#endif