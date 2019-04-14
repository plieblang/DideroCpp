#ifndef _UTILITIES_H
#define _UTILITIES_H

#include "aliases.h"

int convertToNarrowStr(const crs_string &source, char *dest, size_t destSize);
void stripQuotationMarks(char *str);
crs_string constructURL(const crs_string forexUrl, const crs_string forexApiKey, const crs_string &firstCurrency, const crs_string &secondCurrency);

#endif