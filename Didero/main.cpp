#include "main.h"

#define WRITE_INTERVAL 1000
#define FETCH_INTERVAL 1000
#define FETCHES_PER_WRITE WRITE_INTERVAL / FETCH_INTERVAL

int initializeDb(MYSQL *connection) {
	int rv = 0;// mysql_query(connection, "CREATE DATABASE quotedb");
	if (!rv) {
		mysql_query(connection, "DROP TABLE IF EXISTS datatable");
		return mysql_query(connection, "CREATE TABLE datatable(symbol TEXT, low DOUBLE, high DOUBLE, open DOUBLE, close DOUBLE, time BIGINT)");
	}
	return -1;
}

crs_string constructURL(const crs_string forexUrl, const crs_string forexApiKey, const crs_string &firstCurrency, const crs_string &secondCurrency) {
	return forexUrl + U("&from_symbol=") + firstCurrency + U("&to_symbol=") + secondCurrency + U("&interval=1min&apikey=") + forexApiKey;
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
		//https://stackoverflow.com/questions/23310511/mysql-asynchronous
		//Every second, create a list of tasks to be run at increasing intervals in the future
		//Once all those tasks have returned, get the data from them and write it to the db
		std::vector<pplx::task<void>> taskList(FETCHES_PER_WRITE);

		//FIXME do actual error handling
		while (true) {
			try {
				DbData dbData;
				for (int i = 0; i < FETCHES_PER_WRITE; i++) {
					web::uri url(constructURL(forexUrl, forexApiKey, U("USD"), U("EUR")));
					taskList[i] = storeFromQuoteAfterDelay(connection, url, dbData, FETCH_INTERVAL * i);
				}
				auto fetchTasks = pplx::when_all(begin(taskList), end(taskList));
				fetchTasks.wait();
				//initialize the data structure to write data
				//then actually write data
				InsertionQuery iq(dbData);
				DeletionQuery dq(dbData.time);
				std::mutex mut;
				//FIXME needs error checking
				pplx::task<void> writeTask([&iq, &dq, &connection, &mut] {
					iq.execute(connection, mut);
					dq.execute(connection, mut);
					});
			} catch (...) {
				//FIXME log or do something here
			}
		}
	}

	mysql_close(connection);
}