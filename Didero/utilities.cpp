#include "aliases.h"

int convertToNarrowStr(const crs_string &source, char *dest, size_t destSize) {
	size_t rv;
	return wcstombs_s(&rv, dest, destSize, source.c_str(), destSize - 1);
}

void stripQuotationMarks(char *str) {
	int len = strlen(str);
	strcpy_s(str, len, str + 1);
	str[strlen(str) - 1] = 0;
}