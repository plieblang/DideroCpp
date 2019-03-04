#include "main.h"

int initializeDb(MYSQL *connection) {
	int rv = mysql_query(connection, "CREATE DATABASE quotedb");
	if (!rv) {
		return mysql_query(connection, "CREATE TABLE quotetable(symbol VARCHAR(6), price DECIMAL, bid DECIMAL, ask DECIMAL, time TIMESTAMP)");
	}
	return -1;
}

int main() {
	crs_string forexUrl, forexApiKey, dbUrl, dbUsername, dbPassword;
	utility::ifstream_t in("connection_info.txt");
	in >> forexUrl >> forexApiKey >> dbUrl >> dbUsername >> dbPassword;
	DbInfo db(dbUrl, dbUsername, dbPassword);

	MYSQL connection;
	MYSQL *connectionPtr = mysql_init(&connection);
	if (connectionPtr) {
		connectionPtr = mysql_real_connect(connectionPtr, db.url, db.username, db.password, NULL, DB_PORT, NULL, 0);
	}

	int rv = -1;
	if (connectionPtr) {
		rv = initializeDb(connectionPtr);
	}

	if (!rv) {
		storeQuote(forexUrl, forexApiKey).wait();
		//while (true) {
		//	storeQuote(forexUrl, forexApiKey).wait();
		//}
	}

	mysql_close(&connection);
}