#pragma once

#include "base/wbase.h"
#include "wmac.h"
#include "wip.h"

struct WIntf {
	friend struct WIntfList;
	friend struct WNetInfo;

protected:
	int index_{-1};
	std::string name_;
	std::string desc_;
	WMac mac_{WMac::nullMac()};
	WIp ip_{0};
	WIp mask_{0};
	WIp gateway_{0};

protected:
	WIp ip_and_mask_{0}; // used for isSameLanIP

public:
	int index() const { return index_; }
	std::string name() const { return name_; }
	std::string desc() const { return desc_; }
	WMac mac() const { return mac_; }
	WIp ip() const { return ip_; }
	WIp mask() const { return mask_; }
	WIp gateway() const { return gateway_; }
};
