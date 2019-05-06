#ifndef _DB_DATA_H
#define _DB_DATA_H

#include "aliases.h"

#define NUM_QUOTE_PROPERTIES 4
#define SYMBOL_BUF_LEN 7

/*
If low == 0, data is invalid
*/
class DbData {
public:
	char symbol[SYMBOL_BUF_LEN];
	double low, high, open, close;
	time_t time;

	static const crs_string fields[NUM_QUOTE_PROPERTIES];

	DbData() {
		memset(symbol, 0, SYMBOL_BUF_LEN);
		low = high = open = close = 0;
		time = 0;
	}

	DbData(double low, double high, double open, double close, time_t timestamp, const char symbol[SYMBOL_BUF_LEN]) {
		this->low = low;
		this->high = high;
		this->open = open;
		this->close = close;
		this->time = timestamp;
		strcpy_s(this->symbol, symbol);
	}

	//void setProperties(const Quote &q) {
	//	strcpy_s(symbol, q.getSymbol());

	//	double price = q.getPrice();
	//	if (!open) {
	//		open = price;
	//	}
	//	close = price;
	//	if (price > high) {
	//		high = price;
	//	}
	//	if (price < low) {
	//		low = price;
	//	}

	//	time = q.getTimestamp();
	//}
};

#endif