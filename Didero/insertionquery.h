#ifndef _INSERT_QUERY_H
#define _INSERT_QUERY_H

#include "aliases.h"
#include "dbdata.h"
#include "queryfactory.h"

class InsertionQuery : QueryFactory {
	//char query[MAX_QUERY_LEN];

	void constructQuery(const DbData &dbData);

	bool execute(MYSQL* connection);

public:
	static std::mutex iqMut;
	char query[MAX_QUERY_LEN];

	InsertionQuery(const DbData &dbData);
};

#endif