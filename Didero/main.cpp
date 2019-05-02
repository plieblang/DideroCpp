#include "main.h"

#define WRITE_INTERVAL 1000
#define FETCH_INTERVAL 500
#define FETCHES_PER_WRITE WRITE_INTERVAL / FETCH_INTERVAL

int initializeDb(MYSQL* connection) {
	int rv = 0;// mysql_query(connection, "CREATE DATABASE quotedb");
	if (!rv) {
		mysql_query(connection, "DROP TABLE IF EXISTS datatable");
		return mysql_query(connection, "CREATE TABLE datatable(symbol TEXT, low DOUBLE, high DOUBLE, open DOUBLE, close DOUBLE, time BIGINT)");
	}
	return -1;
}

int main() {
	crs_string forexUrl, forexApiKey, dbUrl, dbUsername, dbPassword;
	utility::ifstream_t in("connection_info.txt");
	in >> forexUrl >> forexApiKey >> dbUrl >> dbUsername >> dbPassword;
	DbInfo db(dbUrl, dbUsername, dbPassword);

	MYSQL* connection = mysql_init(NULL);
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
		//put write queries in a queue and execute anything in the queue as soon as possible
		//will need to mutex the actual request
		//eg https://stackoverflow.com/questions/11805232/multi-threaded-c-message-passing
		std::queue<InsertionQuery> writeQueue;
		std::mutex dumbMut;

		while (true) {
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
			//FIXME needs error checking
			pplx::task<void> writeTask([iq, &connection, &dumbMut] {
				//iq.execute(connection, dumbMut);
				std::lock_guard<std::mutex> lg(dumbMut);
				mysql_query(connection, iq.query);
				});
			writeTask.then([dq, &connection, &dumbMut] {
				std::lock_guard<std::mutex> lg(dumbMut);
				mysql_query(connection, dq.query);
				});
			//writeQueue.emplace(iq);
			//if (iq.execute(connection)) {
			//	std::cout << mysql_error(connection);
			//}
		}
	}

	mysql_close(connection);
}