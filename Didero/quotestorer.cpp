#include "quotestorer.h"
#include "main.h"

const crs_string Quote::fields[] = { U("symbol"), U("price"), U("bid"), U("ask"), U("timestamp") };
MYSQL connection;

//mysql_query(&connection, "CREATE DATABASE quotedb");
//mysql_query(&connection, "DROP TABLE IF EXISTS quotetable");
//int rv = mysql_query(&connection, "CREATE TABLE quotetable(symbol VARCHAR(6), price DECIMAL, bid DECIMAL, ask DECIMAL, time TIMESTAMP)");
void storeQuoteInDB(const Quote &quote) {
	InsertionQuery iq(quote, "insert");
	mysql_query(&connection, iq.getQuery());
}

crs_string constructURL(const crs_string forexUrl, const crs_string forexApiKey, const crs_string &firstCurrency, const crs_string &secondCurrency) {
	return forexUrl + firstCurrency + secondCurrency + forexApiKey;
}

Quote createQuote(pplx::task<web::json::value> &previousTask) {
	web::json::value jsonData = previousTask.get();
	crs_string symbol;
	double price, bid, ask;
	time_t timestamp;

	auto entry = jsonData[0];
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
	return q;
}

pplx::task<void> storeQuote(const crs_string &forexUrl, const crs_string &forexApiKey) {
	web::uri url(constructURL(forexUrl, forexApiKey, U("USD"), U("EUR")));
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
	}).then([](pplx::task<web::json::value> previousTask) {
		storeQuoteInDB(createQuote(previousTask));
	});
}