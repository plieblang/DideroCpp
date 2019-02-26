#ifndef _QUERY_BUILDER_H
#define _QUERY_BUILDER_H

#include "aliases.h"

#define MAX_QUERY_LEN 1024

class QueryFactory {
public:
	virtual char *getQuery() = 0;
};

#endif