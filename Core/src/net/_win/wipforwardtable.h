#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "base/wbase.h"

struct WIpForwardTable {
private: // singleton
	WIpForwardTable();
	virtual ~WIpForwardTable();

protected:
	friend struct WRtm;
	PMIB_IPFORWARDTABLE ipForwardTable_{nullptr};

public:
	static WIpForwardTable& instance() {
		static WIpForwardTable ipForwardTable;
		return ipForwardTable;
	}
};
