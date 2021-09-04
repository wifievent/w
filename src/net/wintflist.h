#pragma once

#include <list>
#include "wintf.h"

struct WIntfList : std::list<WIntf> {
	friend struct WNetInfo;

private: // singletin
	WIntfList();

public:
	WIntf* findByName(std::string name);
	WIntf* findByIp(WIp ip);
	static WMac getMac(char* intfName);
};
