#pragma once

#include <string>
#include <cstdarg>
#include <cstring>
#include "gtrace.h"

typedef void *pvoid;
typedef char *pchar;
typedef unsigned char byte;
typedef unsigned char *pbyte;

std::string wformat(const char *fmt, ...);

#define SET_ERR(MSG) { \
	if (err_ == "") { \
		err_ = MSG; \
		GTRACE("%s", err_.data()); \
	} \
}
