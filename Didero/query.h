#ifndef _QUERY_BUILDER_H
#define _QUERY_BUILDER_H

#include "aliases.h"

#define MAX_QUERY_LEN 1024

class Query {
protected:
	char query[MAX_QUERY_LEN];

public:
	int execute(MYSQL *connection) {
		return mysql_query(connection, query);
	}
};

#endif