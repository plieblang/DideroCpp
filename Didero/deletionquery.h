#ifndef _DELETE_QUERY_H
#define _DELETE_QUERY_H

#include "aliases.h"
#include "query.h"

class DeletionQuery : public Query {
	int maxAge = 1000;

	void constructQuery(const time_t currentTime);

public:
	DeletionQuery(const time_t currentTime);
};

#endif