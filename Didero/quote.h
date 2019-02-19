#ifndef _QUOTE_H
#define _QUOTE_H

#include "aliases.h"

#define NUM_QUOTE_PROPERTIES 5
#define SYMBOL_BUF_LEN 16

class Quote {
	char symbol[SYMBOL_BUF_LEN];
	double price, bid, ask;
	time_t time;

	size_t convertToNarrowStr(const crs_string &wsource, char *dest, size_t destSize) {
		size_t rv;
		return wcstombs_s(&rv, dest, destSize, wsource.c_str(), destSize - 1);
		return rv;
	}

public:
	Quote(crs_string symbol, double price, double bid, double ask, time_t time) {
		convertToNarrowStr(symbol, this->symbol, SYMBOL_BUF_LEN);
		this->price = price;
		this->bid = bid;
		this->ask = ask;
		this->time = time;
	}

	static const crs_string fields[NUM_QUOTE_PROPERTIES];

	const char *getSymbol() const {
		const char *rv = symbol;
		return rv;
	}

	double getPrice() const {
		return price;
	}

	double getBid() const {
		return bid;
	}

	double getAsk() const {
		return ask;
	}

	time_t getTimestamp() const {
		return time;
	}
};

#endif