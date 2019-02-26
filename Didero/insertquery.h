#ifndef _INSERT_QUERY_H
#define _INSERT_QUERY_H

#include "aliases.h"
#include "queryfactory.h"
#include "quote.h"

class InsertQuery : QueryFactory {
	char query[MAX_QUERY_LEN];

	int constructQuery(const Quote &quote, const char *givenQuery);

public:
	InsertQuery(const Quote &quote, const char *givenQuery);

	char *getQuery();
};

#endif