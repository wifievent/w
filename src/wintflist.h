#pragma once

#include <list>
#include "wintf.h"

struct WIntfList : std::list<WIntf> {
	WIntfList();

	static WMac getMac(char* intfName);
};
