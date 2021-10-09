#include "wintf.h"

struct WRtmEntry {
	friend struct WRtm;
	friend struct WNetInfo;

	WIp dst() const { return dst_; }
	WIp mask() const { return mask_; }
	WIp gateway() const { return gateway_; }
	int metric() const { return metric_; }
	WIntf* intf() const { return intf_; }

protected:
	WIp dst_{0};
	WIp mask_{0};
	WIp gateway_{0};
	int metric_{0};
	WIntf* intf_{nullptr};
	std::string intfName_;
};
