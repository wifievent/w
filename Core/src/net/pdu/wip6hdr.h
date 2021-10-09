#pragma once

#include "wpdu.h"

#pragma pack(push, 1)
struct WIp6Hdr final {
	uint8_t v_pr_;
	uint8_t flags_[3];
	uint16_t en_;
	uint8_t nh_;
	uint8_t hl_;
	WIp6 sip_;
	WIp6 dip_;

	uint8_t v() { return (v_pr_ & 0xF0) >> 4; }
	uint8_t pr() { return v_pr_ & 0x0F; }
	uint8_t* flags() { return flags_; }
	uint8_t nh() { return nh_; }
	uint8_t hl() { return hl_; }
	WIp6 sip() { return sip_; }
	WIp6 dip() { return dip_; }
};
typedef WIp6Hdr *PIp6Hdr;
#pragma pack(pop)
