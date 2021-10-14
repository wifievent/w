#pragma once

#include "wpdu.h"

#pragma pack(push, 1)
struct WArpHdr final {
	uint16_t hrd_;
	uint16_t pro_;
	uint8_t hln_;
	uint8_t pln_;
	uint16_t op_;
	WMac smac_;
	WIp sip_;
	WMac tmac_;
	WIp tip_;

	uint16_t hrd() { return ntohs(hrd_); }
	uint16_t pro() { return ntohs(pro_); }
	uint8_t hln() { return hln_; }
	uint8_t pln() { return pln_; }
	uint16_t op() { return ntohs(op_);}
	WMac smac() { return smac_; }
	WIp sip() { return ntohl(sip_); }
	WMac tmac() { return tmac_; }
	WIp tip() { return ntohl(tip_); }

	// HardwareType(hrd_)
	enum: uint16_t {
		NETROM = 0, // from KA9Q: NET/ROM pseudo
		ETHER = 1, // Ethernet 10Mbps
		EETHER = 2, // Experimental Ethernet
		AX25 = 3, // AX.25 Level 2
		PRONET = 4, // PROnet token ring
		CHAOS = 5, // Chaosnet
		IEEE802 = 6, // IEEE 802.2 Ethernet/TR/TB
		ARCNET = 7, // ARCnet
		APPLETLK = 8, // APPLEtalk
		LANSTAR = 9, // Lanstar
		DLCI = 15, // Frame Relay DLCI
		ATM = 19, // ATM
		METRICOM = 23, // Metricom STRIP (new IANA id)
		IPSEC = 31 // IPsec tunnel
	};

	// Operation(op_)
	enum: uint16_t {
		Request = 1, // req to resolve address
		Reply = 2, // resp to previous request
		RevRequest = 3, // req protocol address given hardware
		RevReply = 4, // resp giving protocol address
		InvRequest = 8, // req to identify peer
		InvReply = 9 // resp identifying peer
	};
};
typedef WArpHdr *PArpHdr;
#pragma pack(pop)
