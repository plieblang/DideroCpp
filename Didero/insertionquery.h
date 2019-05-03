#ifndef _INSERT_QUERY_H
#define _INSERT_QUERY_H

#include "aliases.h"
#include "dbdata.h"
#include "query.h"

class InsertionQuery : public Query {

	void constructQuery(const DbData &dbData);

public:
	InsertionQuery(const DbData &dbData);
};

#endif