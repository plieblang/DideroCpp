#include "aliases.h"

size_t convertToNarrowStr(const crs_string &wsource, char *dest, size_t destSize) {
	size_t rv;
	return wcstombs_s(&rv, dest, destSize, wsource.c_str(), destSize - 1);
	return rv;
}