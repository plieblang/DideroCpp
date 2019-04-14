#ifndef _DB_DATA_H
#define _DB_DATA_H

#include "aliases.h"
#include "quote.h"

class DbData {
public:
	char symbol[SYMBOL_BUF_LEN];
	double low, high, open, close;
	time_t time;

	DbData() {
		open = close = high = 0;
		low = INT_MAX;
		time = 0;
	}

	void setProperties(const Quote &q) {
		strcpy_s(symbol, q.getSymbol());

		double price = q.getPrice();
		if (!open) {
			open = price;
		}
		close = price;
		if (price > high) {
			high = price;
		}
		if (price < low) {
			low = price;
		}

		time = q.getTimestamp();
	}
};

#endif