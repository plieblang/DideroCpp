#include "quotefetcher.h"
#include "main.h"

const crs_string Quote::fields[] = { U("symbol"), U("price"), U("bid"), U("ask"), U("timestamp") };

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

pplx::task<void> storeFromQuoteAfterDelay(MYSQL *connection, const web::uri &url, DbData &dbData, int milliDelay) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliDelay));

	web::http::client::http_client client(url);
	web::http::http_request request;

	return client.request(request).then([](web::http::http_response response) -> pplx::task<web::json::value> {
		if (response.status_code() == web::http::status_codes::OK) {
			return response.extract_json();
		} else {
			return pplx::task_from_result(web::json::value());
		}
		}).then([&connection, &dbData](pplx::task<web::json::value> previousTask) {
			dbData.setProperties(createQuote(previousTask));
			});
}