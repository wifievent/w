#pragma once

#include "wcapture.h"

struct WPcapCapture : WCapture {
	std::string filter_;
	int mtu_;

	void load(JsonValue& json) override;
	void save(JsonValue& json) override;

public:
	WPcapCapture() {}
	~WPcapCapture() override {}

protected:
	bool doOpen() override;
	bool doClose() override;

public:
	WPacket::Result read(WPacket* packet) override;
	WPacket::Result write(WBuf buf) override;
	WPacket::Result write(WPacket* packet) override;
	WPacket::Result relay(WPacket* packet) override;

protected:
	bool pcapProcessFilter(pcap_if_t* dev);
	pcap_t* pcap_{nullptr};
};
