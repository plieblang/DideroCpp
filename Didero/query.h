#ifndef _QUERY_BUILDER_H
#define _QUERY_BUILDER_H

#include "aliases.h"

#define MAX_QUERY_LEN 1024

class Query {
protected:
	char query[MAX_QUERY_LEN];

public:
	pplx::task<int> execute(MYSQL *connection) {
		auto rv = [&connection, this]() {
			return mysql_query(connection, query);
		};
		return pplx::task_from_result(rv());
	}
};

#endif