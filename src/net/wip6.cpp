#include "wip6.h"
#ifdef WOS_LINUX
#include <arpa/inet.h>
#endif // WOS_LINUX

WIp6::WIp6(const std::string& r) {
	char* p = const_cast<char*>(r.data());

#ifdef WOS_LINUX
	int res = inet_pton(AF_INET6, p, &ip6_);
	if (res == 1) { // succeed
	} else { // fail
		switch (res) {
			case 0:
				fprintf(stderr, "inet_pton return zero ip=%s", r.data());
				break;
			default:
				fprintf(stderr, "inet_pton return %d ip=%s", res, r.data());
				break;
		}
		memset(ip6_, 0, SIZE);
	}
#endif // WOS_LINUX
#ifdef WOS_WIN
	(void)s;(void)p;
#endif // WOS_WIN
}

WIp6::operator std::string() const {
#ifdef WOS_LINUX
	char s[INET6_ADDRSTRLEN];
	const char* res = inet_ntop(AF_INET6, &ip6_, s, INET6_ADDRSTRLEN);
	if (res == nullptr) {
		fprintf(stderr, "inet_ntop return null error=%s", strerror(errno));
		return std::string();
	}
	return std::string(s);
#endif // WOS_LINUX
#ifdef WOS_WIN
	return QString(); // gilgil temp 2010.10.20
#endif // WOS_WIN
}
