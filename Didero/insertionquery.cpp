#include "insertionquery.h"

InsertionQuery::InsertionQuery(const DbData &dbData) {
	constructQuery(dbData);
}

void InsertionQuery::constructQuery(const DbData &dbData) {
	constexpr size_t bufferSize = 33;
	char buf[bufferSize];

	strcpy_s(query, MAX_QUERY_LEN, "INSERT INTO datatable VALUES('");

	strcat_s(query, MAX_QUERY_LEN, dbData.symbol);
	strcat_s(query, MAX_QUERY_LEN, "', ");

	snprintf(buf, bufferSize, "%f", dbData.low);
	strcat_s(query, MAX_QUERY_LEN, buf);
	strcat_s(query, MAX_QUERY_LEN, ", ");

	snprintf(buf, bufferSize, "%f", dbData.high);
	strcat_s(query, MAX_QUERY_LEN, buf);
	strcat_s(query, MAX_QUERY_LEN, ", ");

	snprintf(buf, bufferSize, "%f", dbData.open);
	strcat_s(query, MAX_QUERY_LEN, buf);
	strcat_s(query, MAX_QUERY_LEN, ", ");

	snprintf(buf, bufferSize, "%f", dbData.close);
	strcat_s(query, MAX_QUERY_LEN, buf);
	strcat_s(query, MAX_QUERY_LEN, ", ");

	snprintf(buf, bufferSize, "%lld", dbData.time);
	strcat_s(query, MAX_QUERY_LEN, buf);

	strcat_s(query, MAX_QUERY_LEN, ")");
}

bool InsertionQuery::execute(MYSQL *connection) {
	//std::lock_guard<std::mutex> lg(mtx);
	if (!mysql_query(connection, query)) {
		return true;
	}
	return false;
}