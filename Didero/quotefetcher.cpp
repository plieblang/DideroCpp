#include "insertquery.h"
#include "quote.h"
#include <mysql/mysql.h>

#define DB_PORT 3306
#define TABLE_NAME "deleteme"

const crs_string Quote::fields[] = { U("symbol"), U("price"), U("bid"), U("ask"), U("timestamp") };

static crs_string forexUrl, forexApiKey;
static crs_string dbUrl, dbUsername, dbPassword;

void setData() {
	utility::ifstream_t in("connection_info.txt");
	in >> forexUrl >> forexApiKey >> dbUrl >> dbUsername >> dbPassword;
}

crs_string constructURL(const crs_string &firstCurrency, const crs_string &secondCurrency) {
	return forexUrl + firstCurrency + secondCurrency + forexApiKey;
}

pplx::task<void> getQuotes(std::vector<Quote> &quotes) {
	web::uri url(constructURL(U("USD"), U("EUR")));
	web::http::client::http_client client(url);
	web::http::http_request request;

	return client.request(request).then([](web::http::http_response response) -> pplx::task<web::json::value> {
		// If the status is OK extract the body of the response into a JSON value
		if (response.status_code() == web::http::status_codes::OK) {
			return response.extract_json();
		} else {
			// return an empty JSON value
			return pplx::task_from_result(web::json::value());
		}
	}).then([&quotes](pplx::task<web::json::value> previousTask) {
		crs_json_value jsonData = previousTask.get();
		for (int i = 0; i < jsonData.size(); i++) {
			crs_string symbol;
			double price, bid, ask;
			time_t timestamp;

			auto entry = jsonData[i];
			for (auto prop : Quote::fields) {
				auto data = entry[prop];
				utility::stringstream_t stream;
				data.serialize(stream);

				if (prop == U("symbol")) {
					stream >> symbol;
				} else if (prop == U("price")) {
					stream >> price;
				} else if (prop == U("bid")) {
					stream >> bid;
				} else if (prop == U("ask")) {
					stream >> ask;
				} else stream >> timestamp;
			}

			Quote q(symbol, price, bid, ask, timestamp);
			quotes.emplace_back(q);
		}
	});
}

void storeQuotesInDB(const std::vector<Quote> &quotes) {
	MYSQL connection;
	mysql_init(&connection);
	mysql_real_connect(&connection, DB_URL, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0);
	//mysql_query(&connection, "CREATE DATABASE quotedb");
	mysql_query(&connection, "DROP TABLE IF EXISTS quotetable");
	int rv = mysql_query(&connection, "CREATE TABLE quotetable(symbol VARCHAR(6), price DECIMAL, bid DECIMAL, ask DECIMAL, time TIMESTAMP)");

	for (auto quote : quotes) {
		InsertQuery iq(quote, "insert");
		mysql_query(&connection, iq.getQuery());
	}

	mysql_close(&connection);
}

int main() {
	setData();

	//std::vector<Quote> quotes;
	//getQuotes(quotes).wait();
	//storeQuotesInDB(quotes);
}