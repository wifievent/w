#pragma once

#include "wpdu.h"

#pragma pack(push, 1)
struct WEthHdr final {
	WMac dmac_;
	WMac smac_;
	uint16_t type_;

	WMac dmac() { return dmac_; }
	WMac smac() { return smac_; }
	uint16_t type() { return ntohs(type_); }

	// Type(type_)
	enum: uint16_t {
		Ip4 = 0x0800,
		Arp = 0x0806,
		Ip6 = 0x86DD
	};
};
typedef WEthHdr *PEthHdr;
#pragma pack(pop)
