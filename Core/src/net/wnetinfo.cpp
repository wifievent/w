#include "wnetinfo.h"

WNetInfo::WNetInfo() {
	for (WRtmEntry& entry: rtm_) {
		std::string intfName = entry.intfName_;
		WIntf* intf = intfList_.findByName(intfName);
		if (intf == nullptr) {
			GTRACE("intfList_.findByName(%s) return null", intfName.data());
		}
		entry.intf_ = intf;
	}

	for (WIntf& intf: intfList_) {
		intf.gateway_ = rtm_.findGateway(intf.name_, intf.ip_);
	}
}
