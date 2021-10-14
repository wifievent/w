#pragma once

#include "wpdu.h"

#pragma pack(push, 1)
struct WDhcpHdr final {
	uint8_t type_; // Message type
	uint8_t hrd_; // Hardware type
	uint8_t hln_; // Hardware address length
	uint8_t hops_; // Hops
	uint32_t transaction_; // Transaction ID
	uint16_t elapsed_; // Seconds elapsed
	uint16_t bootp_; // Bootp flags
	WIp clientIp_; // Client IP address
	WIp yourIp_; // Your (client) IP address
	WIp serverIp_; // Next srever IP address
	WIp relayIp_; // Relay agent IP address
	WMac clientMac_; // Client MAC address
	uint8_t padding_[10]; // Client hardware address padding
	uint8_t serverHostName_[64]; // Server host name not given
	uint8_t bootFileName_[128]; // Boot file name not given
	uint32_t magic_; // Magic cookie : DHCP

	uint8_t type() { return type_; }
	uint8_t hrd() { return hrd_; }
	uint8_t hln() { return hln_; }
	uint8_t hops() { return hops_; }
	uint32_t transaction() { return ntohl(transaction_); }
	uint16_t elapsed() { return ntohs(elapsed_); }
	uint16_t bootp() { return ntohs(bootp_); }
	WIp clientIp() { return ntohl(clientIp_); }
	WIp yourIp() { return ntohl(yourIp_); }
	WIp serverIp() { return ntohl(serverIp_); }
	WIp relayIp() { return ntohl(relayIp_); }
	WMac clientMac() { return clientMac_; }
	uint8_t* padding() { return padding_; }
	uint8_t* serverHostName() { return serverHostName_; }
	uint8_t* bootFileName() { return bootFileName_; }
	uint32_t magic() { return ntohl(magic_); }

	struct Option {
		uint8_t type_;
		uint8_t len_;

		Option* next() {
			Option* res = POption((char*)this + len_ + 2);
			if (res->type_ == End) return nullptr;
			return res;
		}

	};
	typedef Option *POption;

	Option* first() {
		return POption((char*)this + sizeof(WDhcpHdr));
	}

	struct OptionRequestIpAddress : Option {
		WIp ip_;
	};
	typedef OptionRequestIpAddress *POptionRequestIpAddress;

	// OptionType(Option::type_)
	enum: uint8_t {
		RequestedIpAddress = 50,
		End = 255
	};
};
typedef WDhcpHdr *PDhcpHdr;
#pragma pack(pop)
