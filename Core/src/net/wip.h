#pragma once

#include <cstdint>
#include <cstring>
#include <string>

struct WIp final {
	static const int SIZE = 4;

	// constructor
	WIp() {}
	WIp(const uint32_t r) : ip_(r) {}
	WIp(const std::string r);

	// casting operator
	operator uint32_t() const { return ip_; } // default
	explicit operator std::string() const;

	// comparison operator
	bool operator == (const WIp& r) const { return ip_ == r.ip_; }

	bool isNull() { return ip_ == 0; }

	bool isLocalHost() const { // 127.*.*.*
		uint8_t prefix = (ip_ & 0xFF000000) >> 24;
		return prefix == 0x7F;
	}

	bool isBroadcast() const { // 255.255.255.255
		return ip_ == 0xFFFFFFFF;
	}

	bool isMulticast() const { // 224.0.0.0 ~ 239.255.255.255
		uint8_t prefix = (ip_ & 0xFF000000) >> 24;
		return prefix >= 0xE0 && prefix < 0xF0;
	}
	static WIp nullIp() { return WIp(0); }

protected:
	uint32_t ip_;
};
typedef WIp *PIp;
