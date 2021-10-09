#pragma once

#include "wpcapcapture.h"
#include "net/wnetinfo.h"

struct WPcapDevice : WPcapCapture {
	std::string intfName_{""};
	int snapLen_{WPacket::MaxBufSize};
	int flags_{1}; // PCAP_OPENFLAG_PROMISCUOUS
	int readTimeout_{-1}; // -1 msec
	int waitTimeout_{1}; // 1msec

	void load(Json::Value& json) override;
	void save(Json::Value& json) override;

public:
	WPcapDevice();
	~WPcapDevice() override { close(); }

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	WPacket::Result read(WPacket* packet) override;
	WIntf* intf() { return intf_; }

protected:
	WIntf* intf_{nullptr};
};
