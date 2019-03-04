#include "aliases.h"

int convertToNarrowStr(const crs_string &source, char *dest, size_t destSize) {
	size_t rv;
	return wcstombs_s(&rv, dest, destSize, source.c_str(), destSize - 1);
}