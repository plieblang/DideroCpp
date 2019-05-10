#include "main.h"

#define WRITE_INTERVAL 180000
#define FETCH_INTERVAL 180000
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
		rv = 0;// initializeDb(connection);
	}

	if (!rv) {
		//https://stackoverflow.com/questions/23310511/mysql-asynchronous
		//Every second, create a list of tasks to be run at increasing intervals in the future
		//Once all those tasks have returned, get the data from them and write it to the db
		std::vector<pplx::task<void>> taskList(FETCHES_PER_WRITE);

		std::mutex mut;

		//FIXME do actual error handling
		while (true) {
			try {
				std::vector<std::optional<DbData>> quotes(FETCHES_PER_WRITE);
				for (int i = 0; i < FETCHES_PER_WRITE; i++) {
					web::uri url(constructURL(forexUrl, forexApiKey, U("USD"), U("EUR")));
					taskList[i] = storeFromQuoteAfterDelay(connection, url, quotes[i], FETCH_INTERVAL * (i + 1));
				}
				auto fetchTasks = pplx::when_all(begin(taskList), end(taskList));
				fetchTasks.wait();

				//set the data that we're actually going to write
				DbData writeData(quotes[0].value());
				bool valid = true;
				for (const auto &quote : quotes) {
					if (quote.has_value()) {
						if (quote.value().low < writeData.low) {
							writeData.low = quote.value().low;
						}
						if (quote.value().high > writeData.high) {
							writeData.high = quote.value().high;
						}
					} else {
						valid = false;
						break;
					}
					
				}
				writeData.close = quotes[quotes.size() - 1].value().close;
				writeData.time = quotes[quotes.size() - 1].value().time;

				//make sure we have valid data
				if (!valid) {
					continue;
				}

				InsertionQuery iq(writeData);
				DeletionQuery dq(writeData.time);
				iq.execute(connection);
				dq.execute(connection);
			} catch (std::exception e) {
				std::cout << e.what() << "\n";
			} catch (...) {
				std::cout << "exception handled\n";
			}
		}
	}

	mysql_close(connection);
}