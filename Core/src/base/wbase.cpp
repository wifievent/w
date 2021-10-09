#include "wbase.h"

std::string wformat(const char *fmt, ...) {
	char temp[BUFSIZ];
	va_list args;
	va_start(args, fmt);
	int res = vsnprintf(temp, BUFSIZ, fmt, args);
	va_end(args);
	if (res < 0) {
		fprintf(stderr, "vsnprintf: snprintf return %d\n", res);
		return std::string();
	}
	return temp;
}

