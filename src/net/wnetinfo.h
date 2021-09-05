#pragma once

#include "wintflist.h"
#include "wrtm.h"

struct WNetInfo {
private: // singleton
	WNetInfo();
	virtual ~WNetInfo() {}

	WIntfList intfList_;
	WRtm rtm_;

public:
	WIntfList& intfList() { return intfList_; }
	WRtm& rtm() { return rtm_; }

	static WNetInfo& instance() {
		static WNetInfo netInfo;
		return netInfo;
	}
};
