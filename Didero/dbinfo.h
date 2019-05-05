#ifndef _DB_INFO_H
#define _DB_INFO_H

#include "aliases.h"

#define DB_INFO_STRLEN 128

typedef struct DbInfo {
	char url[DB_INFO_STRLEN];
	char username[DB_INFO_STRLEN];
	char password[DB_INFO_STRLEN];

	DbInfo(crs_string url, crs_string user, crs_string pwd) {
		auto narrowStrConverter = [](const crs_string & source, char *dest, size_t destSize) {
			size_t rv;
			return wcstombs_s(&rv, dest, destSize, source.c_str(), destSize - 1);
		};
		narrowStrConverter(url, this->url, DB_INFO_STRLEN);
		narrowStrConverter(user, this->username, DB_INFO_STRLEN);
		narrowStrConverter(pwd, this->password, DB_INFO_STRLEN);
	};
} DbInfo;

#endif