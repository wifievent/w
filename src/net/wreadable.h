#pragma once

#include "net/packet/wpacket.h"

struct WReadable {
	virtual WPacket::Result read(WPacket* packet) = 0;
};
