#pragma once

#include <winsock2.h>
#include <iphlpapi.h>

#include "base/wbase.h"

struct WIpAdapterInfo {
private: // singleton
	WIpAdapterInfo();
	virtual ~WIpAdapterInfo();

protected:
	PIP_ADAPTER_INFO pAdapterInfo_{nullptr};

public:
	PIP_ADAPTER_INFO findByAdapterName(char* value);
	PIP_ADAPTER_INFO findByComboIndex(DWORD comboIndex);

	static WIpAdapterInfo& instance() {
		static WIpAdapterInfo ipAdapterInfo;
		return ipAdapterInfo;
	}
};

