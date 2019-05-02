#ifndef _DELETE_QUERY_H
#define _DELETE_QUERY_H

#include "aliases.h"
#include "queryfactory.h"

class DeletionQuery : QueryFactory {
	//char query[MAX_QUERY_LEN];

	int maxAge = 1000;

	void constructQuery(const time_t currentTime);

	bool execute(MYSQL* connection);

public:
	static std::mutex iqMut;
	char query[MAX_QUERY_LEN];

	DeletionQuery(const time_t currentTime);
};

#endif