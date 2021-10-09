#include <iostream>
#include "net/wnetinfo.h"

using namespace std;

int main() {
	WNetInfo& netInfo = WNetInfo::instance();
	WIntfList& intfList = netInfo.intfList();
	WRtm& rtm = netInfo.rtm();
	WRtmEntry* entry = rtm.getBestEntry(WIp("8.8.8.8"));

	for (WIntf& intf : intfList){
		bool best = entry != nullptr &&
			entry->intf() != nullptr &&
			entry->intf()->name() == intf.name();
		printf("index=%d %s\n", intf.index(), best ? "(Best)" : " ");
		printf("  name=%s desc=%s\n", intf.name().data(), intf.desc().data());
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

	printf("dst\tmask\tgateway\tmetric\tintf\n");
	for (WRtmEntry& entry: rtm) {
		printf("%s\t%s\t%s\t%d\t%s\n",
			std::string(entry.dst()).data(),
			std::string(entry.mask()).data(),
			std::string(entry.gateway()).data(),
			entry.metric(),
			std::string(entry.intf()->desc()).data());
	}
}
