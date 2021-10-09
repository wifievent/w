#pragma once

#include <list>
#include "wrtmentry.h"

struct WRtm : std::list<WRtmEntry> { // routing table manager
	friend struct WRtmEntry;
	friend struct WNetInfo;

private: // singleton
	WRtm();
	virtual ~WRtm() { clear(); }

public:
	WRtmEntry* getBestEntry(WIp ip);
	WIp findGateway(std::string intfName, WIp ip);

#ifdef WOS_LINUX
protected:
	static bool checkA(char* buf, WRtmEntry* entry);
	static bool checkB(char* buf, WRtmEntry* entry);
	static bool checkC(char* buf, WRtmEntry* entry);
	static bool checkD(char* buf, WRtmEntry* entry);
	static bool decodeCidr(std::string cidr, uint32_t* dst, uint32_t* mask);
	static uint32_t numberToMask(int number);
#endif
};
