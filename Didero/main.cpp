#include "main.h"

int main() {
	crs_string forexUrl, forexApiKey, dbUrl, dbUsername, dbPassword;
	utility::ifstream_t in("connection_info.txt");
	in >> forexUrl >> forexApiKey >> dbUrl >> dbUsername >> dbPassword;
	DbInfo db(dbUrl, dbUsername, dbPassword);

	MYSQL connection;
	mysql_init(&connection);
	mysql_real_connect(&connection, db.url, db.username, db.password, NULL, DB_PORT, NULL, 0);

	std::queue<Quote> quotes;
	while (true) {
		storeQuote(forexUrl, forexApiKey).wait();
		//storeQuotesInDB(quotes, db);
	}

	mysql_close(&connection);
}