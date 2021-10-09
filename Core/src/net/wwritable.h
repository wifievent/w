#pragma once

#include "net/packet/wpacket.h"

struct WWritable {
	~WWritable() {
		if (tempBuf_.data_ != nullptr) {
			delete[] tempBuf_.data_ ;
		}
		tempBuf_.clear();
	}

	virtual WPacket::Result write(WBuf buf) = 0;
	virtual WPacket::Result write(WPacket* packet) = 0;
	WPacket::Result writeMtuSplit(WPacket* packet, size_t mtu);

	WBuf tempBuf_{nullptr, 0};
};
