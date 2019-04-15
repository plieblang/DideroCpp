#ifndef _INSERT_QUERY_H
#define _INSERT_QUERY_H

#include "aliases.h"
#include "dbdata.h"
#include "queryfactory.h"

class InsertionQuery : QueryFactory {
	char query[MAX_QUERY_LEN];

	int constructQuery(const DbData &dbData);

public:
	InsertionQuery(const DbData &dbData);

	char *getQuery() const;
};

#endif