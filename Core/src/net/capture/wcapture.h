#pragma once

#include "base/wstateobj.h"
#include "net/packet/wpacket.h"
#include "net/wreadable.h"
#include "net/wwritable.h"

struct WCapture: WStateObj, WReadable, WWritable {
	WPacket::Result read(WPacket* packet) override;
	WPacket::Result write(WBuf buf) override;
	WPacket::Result write(WPacket* packet) override;
	virtual WPacket::Result relay(WPacket* packet);
	virtual WPacket::Result drop(WPacket* packet);
};
