#pragma once

#include "wiphdr.h"

#pragma pack(push, 1)
struct WUdpHdr final {
	uint16_t sport_;
	uint16_t dport_;
	uint16_t len_;
	uint16_t sum_;

	uint16_t sport() { return ntohs(sport_); }
	uint16_t dport() { return ntohs(dport_); }
	uint16_t len() { return ntohs(len_); }
	uint16_t sum() { return ntohs(sum_); }

	static uint16_t calcChecksum(WIpHdr* ipHdr, WUdpHdr* udpHdr);
	static WBuf parseData(WUdpHdr* udpHdr);
};
typedef WUdpHdr *PUdpHdr;
#pragma pack(pop)
