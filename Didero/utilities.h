#ifndef _UTILITIES_H
#define _UTILITIES_H

#include "aliases.h"

int convertToNarrowStr(const crs_string &source, char *dest, size_t destSize);
void stripQuotationMarks(char *str);

#endif