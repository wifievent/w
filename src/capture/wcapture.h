#pragma once

#include "base/wstateobj.h"
#include "net/packet/wpacket.h"

struct WCapture: WStateObj {
	WPacket::Result read(WPacket* packet) override;
	WPacket::Result write(GBuf buf) override;
	WPacket::Result write(WPacket* packet) override;
	virtual WPacket::Result relay(WPacket* packet);
	virtual WPacket::Result drop(WPacket* packet);
};
