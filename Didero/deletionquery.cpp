#include "deletionquery.h"

DeletionQuery::DeletionQuery(const time_t currentTime) {
	constructQuery(currentTime);
}

void DeletionQuery::constructQuery(const time_t currentTime) {
	constexpr size_t bufferSize = 33;
	char buf[bufferSize];

	strcpy_s(query, MAX_QUERY_LEN, "DELETE FROM datatable WHERE time<");

	snprintf(buf, bufferSize, "%lld", currentTime - maxAge);
	strcat_s(query, MAX_QUERY_LEN, buf);
}