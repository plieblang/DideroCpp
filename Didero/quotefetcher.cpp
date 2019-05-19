#include "quotefetcher.h"

const crs_string DbData::fields[] = { U("1. open"), U("2. high"), U("3. low"), U("4. close") };

time_t convertFriendlyToTimestamp(crs_string &date, const crs_string &time) {
	std::tm formattedTime;
	memset(&formattedTime, 0, sizeof(formattedTime));

	constexpr int tempStrLen = 11;
	wchar_t tempStr[tempStrLen];

	wcscpy_s(tempStr, date.c_str());
	wchar_t *buf = NULL;
	wchar_t *token = wcstok_s(tempStr, L"-", &buf);
	int counter = 0;
	while (token) {
		int val = 0;
		try {
			val = std::stoi(token);
		} catch (std::invalid_argument e) {
			std::cout << "Received data with invalid date: " << token << "\n";
			return -1;
		}

		if (!counter) {
			formattedTime.tm_year = val - 1900;
		} else if (counter == 1) {
			formattedTime.tm_mon = val - 1;
		} else if (counter == 2) {
			formattedTime.tm_mday = val;
		}
		counter++;
		token = wcstok_s(NULL, L"-", &buf);
	}

	wcscpy_s(tempStr, time.c_str());
	buf = NULL;
	token = wcstok_s(tempStr, L":", &buf);
	counter = 0;
	while (token) {
		int val = 0;
		try {
			val = std::stoi(token);
		} catch (std::invalid_argument e) {
			std::cout << "Received data with invalid time: " << token << "\n";
			return -1;
		}

		if (!counter) {
			formattedTime.tm_hour = val;
		} else if (counter == 1) {
			formattedTime.tm_min = val;
		} else if (counter == 2) {
			formattedTime.tm_sec = val;
		}
		counter++;
		token = wcstok_s(NULL, L":", &buf);
	}

	return mktime(&formattedTime);
}

std::optional<DbData> createQuote(pplx::task<web::json::value> &previousTask) {
	web::json::value jsonData = previousTask.get();
	double low, high, open, close;
	time_t timestamp;

	//something happened; probably hit the api limit
	if (!jsonData.has_field(U("Meta Data"))) {
		std::cout << "Received invalid JSON\n";
		utility::stringstream_t stream;
		jsonData.serialize(stream);
		crs_string receivedData;
		stream >> receivedData;
		std::cout << receivedData.c_str() << "\n";

		return std::nullopt;
	}

	auto metadata = jsonData[U("Meta Data")];
	auto interval = metadata[U("4. Last Refreshed")];
	crs_string date, timeStr;

	{
		utility::stringstream_t stream;
		interval.serialize(stream);
		stream >> date;
		stream >> timeStr;
		//strip off the quotation marks that are included for some reason
		date = date.substr(1, date.size() - 1);
		timeStr = timeStr.substr(0, timeStr.size() - 1);
		timestamp = convertFriendlyToTimestamp(date, timeStr);
	}

	if (timestamp < 0) {
		return std::nullopt;
	}

	auto entry = jsonData[U("Time Series FX (1min)")];
	entry = entry[date + U(" ") + timeStr];
	crs_string propStr = U("");
	for (auto prop : DbData::fields) {
		auto data = entry[prop];
		utility::stringstream_t stream;
		data.serialize(stream);
		stream >> propStr;
		propStr = propStr.substr(1, propStr.size() - 2);

		double value = 0;
		try {
			value = std::stod(propStr);
		} catch (std::invalid_argument e) {
			std::cout << e.what();
			return std::nullopt;
		}

		if (prop == U("1. open")) {
			open = value;
		} else if (prop == U("2. high")) {
			high = value;
		} else if (prop == U("3. low")) {
			low = value;
		} else if (prop == U("4. close")) {
			close = value;
		}
	}

	DbData rv(low, high, open, close, timestamp, "USDEUR");
	return rv;
}

pplx::task<void> storeFromQuoteAfterDelay(MYSQL *connection, const web::uri &url, std::optional<DbData> &dbData, int milliDelay) {
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
			dbData = createQuote(previousTask);
			});
}