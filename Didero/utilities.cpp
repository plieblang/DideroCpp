#include "aliases.h"

int convertToNarrowStr(const crs_string &source, char *dest, size_t destSize) {
	size_t rv;
	return wcstombs_s(&rv, dest, destSize, source.c_str(), destSize - 1);
}

void stripQuotationMarks(char *str) {
	size_t len = strlen(str);
	strcpy_s(str, len, str + 1);
	str[strlen(str) - 1] = 0;
}

crs_string constructURL(const crs_string forexUrl, const crs_string forexApiKey, const crs_string &firstCurrency, const crs_string &secondCurrency) {
	return forexUrl + firstCurrency + secondCurrency + forexApiKey;
}