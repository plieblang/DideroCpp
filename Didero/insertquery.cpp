#include "insertquery.h"

InsertQuery::InsertQuery(const Quote &quote, const char *givenQuery) {
	constructQuery(quote, givenQuery);
}

int InsertQuery::constructQuery(const Quote &quote, const char *givenQuery) {
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

char *InsertQuery::getQuery() {
	return query;
}