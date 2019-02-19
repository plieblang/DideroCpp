#ifndef _INSERT_QUERY_H
#define _INSERT_QUERY_H

#include "queryfactory.h"

class InsertQuery : QueryFactory {
	char query[MAX_QUERY_LEN];

	//TODO return actual failure code
	int constructQuery(const Quote &quote, const char *givenQuery) {
		constexpr size_t bufferSize = 32;
		char buf[bufferSize];

		strcat_s(query, MAX_QUERY_LEN, "INSERT INTO quotetable VALUES('");
		strcat_s(query, MAX_QUERY_LEN, quote.getSymbol());
		strcat_s(query, MAX_QUERY_LEN, "',");

		snprintf(buf, bufferSize, "%f", quote.getPrice());
		strcat_s(query, MAX_QUERY_LEN, buf);
		strcat_s(query, MAX_QUERY_LEN, ",");

		snprintf(buf, bufferSize, "%f", quote.getBid());
		strcat_s(query, MAX_QUERY_LEN, buf);
		strcat_s(query, MAX_QUERY_LEN, ",");

		snprintf(buf, bufferSize, "%f", quote.getAsk());
		strcat_s(query, MAX_QUERY_LEN, buf);
		strcat_s(query, MAX_QUERY_LEN, ",");

		snprintf(buf, bufferSize, "%lld", quote.getTimestamp());
		strcat_s(query, MAX_QUERY_LEN, buf);

		return 0;
	}

public:
	InsertQuery(const Quote &quote, const char *givenQuery) {
		constructQuery(quote, givenQuery);
	}

	char *getQuery() {
		return query;
	}
};

#endif