#include "wpcapcapture.h"

void WPcapCapture::load(JsonValue& json) {
	(void)json;
}

void WPcapCapture::save(JsonValue& json) {
	(void)json;
}

bool WPcapCapture::doOpen() {
	if (filter_ != "") {
		if (!pcapProcessFilter(nullptr))
			return false;
	}
	return true;
}

bool WPcapCapture::doClose() {
	if (pcap_ != nullptr) {
		pcap_close(pcap_);
		pcap_ = nullptr;
	}
	return true;
}

WPacket::Result WPcapCapture::read(WPacket* packet) {
	packet->clear();
	pcap_pkthdr* pktHdr;
	int i = pcap_next_ex(pcap_, &pktHdr, const_cast<const u_char**>(&(packet->buf_.data_)));
	if (state_ != Opened) return WPacket::Fail; // may be pcap_close called
	WPacket::Result res;
	switch (i) {
		case PCAP_ERROR: { // if an error occurred
			char* e = pcap_geterr(pcap_);
			if (e != nullptr && strlen(e) > 0) {
				GTRACE("pcap_next_ex return -1 error=%s", e);
			}
			res = WPacket::Eof;
			break;
		}
		case PCAP_ERROR_BREAK: { // if EOF was reached reading from an offline capture
			char* e = pcap_geterr(pcap_);
			if (e != nullptr && strlen(e) > 0) {
				GTRACE("pcap_next_ex return -2 error=%s", e);
			}
			res = WPacket::Eof;
			break;

		}
		case 0 : // if a timeout occured
			res = WPacket::None;
			break;
		default: // packet captured
			packet->ts_ = pktHdr->ts;
			packet->buf_.size_ = pktHdr->caplen;
			packet->parse();
			res = WPacket::Ok;
			break;
	}
	return res;
}

WPacket::Result WPcapCapture::write(WBuf buf) {
	int i = pcap_sendpacket(pcap_, buf.data_, int(buf.size_));
	if (i == 0) return WPacket::Ok;
	char* e = pcap_geterr(pcap_);
	if (e == nullptr) e = pchar("unknown");
	GTRACE("pcap_sendpacket return %d(%s) length=%lu", i, e, buf.size_);
	return WPacket::Fail;
}

WPacket::Result WPcapCapture::write(WPacket* packet) {
	// qDebug() << packet->buf_.size_; // gilgil temp 2021.07.12
	WPacket::Result res;
	if (mtu_ != 0 && packet->buf_.size_ > sizeof(WEthHdr) + mtu_ && packet->tcpHdr_ != nullptr)
		res = writeMtuSplit(packet, mtu_);
	else
		res = write(packet->buf_);
	return res;
}

WPacket::Result WPcapCapture::relay(WPacket* packet) {
	(void)packet;
	SET_ERR("virtual function call");
	return WPacket::Fail;
}

bool WPcapCapture::pcapProcessFilter(pcap_if_t* dev) {
	u_int uNetMask;
	bpf_program code;

	if (dev != nullptr && dev->addresses != nullptr && dev->addresses->netmask != nullptr)
		uNetMask = (reinterpret_cast<struct sockaddr_in*>(dev->addresses->netmask))->sin_addr.s_addr;
	else
		uNetMask = 0xFFFFFFFF;
	if (pcap_compile(pcap_, &code, filter_.data(), 1, uNetMask) < 0) {
		GTRACE("error in pcap_compile(%s)", pcap_geterr(pcap_));
		return false;
	}
	if (pcap_setfilter(pcap_, &code) < 0) {
		GTRACE("error in pcap_setfilter(%s)", pcap_geterr(pcap_));
		return false;
	}
	return true;
}
