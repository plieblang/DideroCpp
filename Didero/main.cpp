#include "main.h"

int main() {
	crs_string forexUrl, forexApiKey, dbUrl, dbUsername, dbPassword;
	utility::ifstream_t in("connection_info.txt");
	in >> forexUrl >> forexApiKey >> dbUrl >> dbUsername >> dbPassword;
	DbInfo db(dbUrl, dbUsername, dbPassword);

	std::queue<Quote> quotes;
	while (true) {
		getQuotes(quotes, forexUrl, forexApiKey).wait();
		//storeQuotesInDB(quotes);
	}
}