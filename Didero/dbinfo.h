#ifndef _DB_INFO_H
#define _DB_INFO_H

#include "aliases.h"
#include "utilities.h"

#define DB_INFO_STRLEN 128

typedef struct DbInfo {
	char url[DB_INFO_STRLEN];
	char username[DB_INFO_STRLEN];
	char password[DB_INFO_STRLEN];

	DbInfo(crs_string url, crs_string user, crs_string pwd) {
		convertToNarrowStr(url, this->url, DB_INFO_STRLEN);
		convertToNarrowStr(user, this->username, DB_INFO_STRLEN);
		convertToNarrowStr(pwd, this->password, DB_INFO_STRLEN);
	};
} DbInfo;

#endif