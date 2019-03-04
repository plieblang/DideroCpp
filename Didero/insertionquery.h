#ifndef _INSERT_QUERY_H
#define _INSERT_QUERY_H

#include "aliases.h"
#include "queryfactory.h"
#include "quote.h"

class InsertionQuery : QueryFactory {
	char query[MAX_QUERY_LEN];

	int constructQuery(const Quote &quote);

public:
	InsertionQuery(const Quote &quote);

	char *getQuery() const;
};

#endif