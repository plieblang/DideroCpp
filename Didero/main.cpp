#include "main.h"

int initializeDb(MYSQL *connection) {
	int rv = 0;// mysql_query(connection, "CREATE DATABASE quotedb");
	if (!rv) {
		mysql_query(connection, "DROP TABLE IF EXISTS quotetable");
		return mysql_query(connection, "CREATE TABLE quotetable(symbol TEXT, price DOUBLE, bid DOUBLE, ask DOUBLE, time BIGINT)");
	}
	return -1;
}

int main() {
	crs_string forexUrl, forexApiKey, dbUrl, dbUsername, dbPassword;
	utility::ifstream_t in("connection_info.txt");
	in >> forexUrl >> forexApiKey >> dbUrl >> dbUsername >> dbPassword;
	DbInfo db(dbUrl, dbUsername, dbPassword);

	MYSQL *connection = mysql_init(NULL);
	if (connection) {
		connection = mysql_real_connect(connection, db.url, db.username, db.password, "quotedb", DB_PORT, NULL, 0);
	}

	int rv = -1;
	if (connection) {
		rv = initializeDb(connection);
	}

	if (!rv) {
		storeQuote(connection, forexUrl, forexApiKey).wait();
		//while (true) {
		//	storeQuote(forexUrl, forexApiKey).wait();
		//}
	}

	mysql_close(connection);
}