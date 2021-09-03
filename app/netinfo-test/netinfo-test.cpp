#include <iostream>
#include "wintflist.h"

using namespace std;

int main() {
	WIntfList intfList;

	for (WIntfList::iterator it = intfList.begin(); it != intfList.end(); it++) {
		WIntf& intf = *it;
		printf("index=%d\n", intf.index());
		printf("  name=%s\n", intf.name().data());
		printf("  desc=%s\n", intf.desc().data());
		if (!intf.mac().isNull())
			printf("  mac=%s\n", std::string(intf.mac()).data());
		if (!intf.ip().isNull())
			printf("  ip=%s\n", std::string(intf.ip()).data());
		if (!intf.mask().isNull())
			printf("  mask=%s\n", std::string(intf.mask()).data());
		if (!intf.gateway().isNull())
			printf("  gateway=%s\n", std::string(intf.gateway()).data());
		printf("\n");
	}
}
