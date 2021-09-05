#include "wpcapdevice.h"

void WPcapDevice::load(JsonValue& json) {
	(void)json;
}

void WPcapDevice::save(JsonValue& json) {
	(void)json;
}

WPcapDevice::WPcapDevice() {
	WRtmEntry* entry = WNetInfo::instance().rtm().getBestEntry(WIp("8.8.8.8"));
	if (entry != nullptr) {
		WIntf* intf = entry->intf();
		if (intf != nullptr)
			intfName_ = intf->name();
	}
}

bool WPcapDevice::doOpen() {
	if (intfName_ == "") {
		GTRACE("intfName is not specified");
		return false;
	}

	char errBuf[PCAP_ERRBUF_SIZE];
	pcap_ = pcap_open_live(intfName_.data(), snapLen_, flags_, readTimeout_, errBuf);
	if (pcap_ == nullptr) {
		GTRACE("pcap_open_live return null err=%s", errBuf);
		return false;
	}

	intf_ = WNetInfo::instance().intfList().findByName(intfName_);
	if (intf_ == nullptr) {
		GTRACE("can not find interface for %s", intfName_.data());
		return false;
	}

	return WPcapCapture::doOpen();
}

bool WPcapDevice::doClose() {
	return WPcapCapture::doClose();
}

WPacket::Result WPcapDevice::read(WPacket* packet) {
	WPacket::Result res = WPcapCapture::read(packet);
	if (res == WPacket::None && waitTimeout_ != 0) usleep(waitTimeout_ * 1000);
	return res;
}
