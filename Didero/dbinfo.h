#ifndef _DB_INFO_H
#define _DB_INFO_H

#include "aliases.h"

#define DB_INFO_STRLEN 128

typedef struct DbInfo {
	char url[DB_INFO_STRLEN];
	char username[DB_INFO_STRLEN];
	char password[DB_INFO_STRLEN];
} DbInfo;

#endif