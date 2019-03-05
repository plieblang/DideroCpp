#include "insertionquery.h"

InsertionQuery::InsertionQuery(const Quote &quote) {
	constructQuery(quote);
}

int InsertionQuery::constructQuery(const Quote &quote) {
	constexpr size_t bufferSize = 32;
	char buf[bufferSize];

	strcpy_s(query, MAX_QUERY_LEN, "INSERT INTO quotetable VALUES('");

	strcat_s(query, MAX_QUERY_LEN, quote.getSymbol());
	strcat_s(query, MAX_QUERY_LEN, "', ");

	snprintf(buf, bufferSize, "%f", quote.getPrice());
	strcat_s(query, MAX_QUERY_LEN, buf);
	strcat_s(query, MAX_QUERY_LEN, ", ");

	snprintf(buf, bufferSize, "%f", quote.getBid());
	strcat_s(query, MAX_QUERY_LEN, buf);
	strcat_s(query, MAX_QUERY_LEN, ", ");

	snprintf(buf, bufferSize, "%f", quote.getAsk());
	strcat_s(query, MAX_QUERY_LEN, buf);
	strcat_s(query, MAX_QUERY_LEN, ", ");

	snprintf(buf, bufferSize, "%lld", quote.getTimestamp());
	strcat_s(query, MAX_QUERY_LEN, buf);

	strcat_s(query, MAX_QUERY_LEN, ")");

	return 0;
}

char *InsertionQuery::getQuery() const {
	return (char*)query;
}