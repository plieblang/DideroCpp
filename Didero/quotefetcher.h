#ifndef _QUOTE_FETCHER_H
#define _QUOTE_FETCHER_H

#include "aliases.h"
#include "dbdata.h"
#include "dbinfo.h"

pplx::task<void> storeFromQuoteAfterDelay(MYSQL *connection, const web::uri &url, std::optional<DbData> &dbData, int milliDelay);

#endif