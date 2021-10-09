#pragma once

#include "base/wstateobj.h"
#include "net/packet/wpacket.h"

struct WPacketDbg : WStateObj {
protected:
	bool doOpen() override { return true; }
	bool doClose() override { return true; }

public:
	void debug(WPacket* packet);
};
