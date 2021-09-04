#pragma once

#include <cstdint>
#include <cstring>
#include <string>

struct WIp6 final {
	static constexpr int SIZE = 16;

	// constructor
	WIp6() {}
	WIp6(const WIp6& r) { memcpy(ip6_, r.ip6_, SIZE); }
	WIp6(const uint8_t* r) { memcpy(ip6_, r, SIZE); }
	WIp6(const std::string& r);

	// assign operator
	WIp6& operator = (const WIp6& r) { memcpy(this->ip6_, r.ip6_, SIZE); return *this; }

	// casting operator
	operator uint8_t*() const { return const_cast<uint8_t*>(ip6_); } // default casting operator
	explicit operator std::string() const;

	// comparison operator
	bool operator == (const WIp6& r) const { return memcmp(ip6_, r.ip6_, SIZE) == 0; }
	bool operator != (const WIp6& r) const { return memcmp(ip6_, r.ip6_, SIZE) != 0; }
	bool operator < (const WIp6& r) const { return memcmp(ip6_, r.ip6_, SIZE) < 0; }
	bool operator > (const WIp6& r) const { return memcmp(ip6_, r.ip6_, SIZE) > 0; }
	bool operator <= (const WIp6& r) const { return memcmp(ip6_, r.ip6_, SIZE) <= 0; }
	bool operator >= (const WIp6& r) const { return memcmp(ip6_, r.ip6_, SIZE) >= 0; }
	bool operator == (const u_char* r) const { return memcmp(ip6_, r, SIZE) == 0; }

	void clear() {
		memset(ip6_, 0, SIZE);
	}

	bool isLocalHost() {
		return true; // gilgil temp 2019.05.11
	}

	bool isBroadcast() {
		return true; // gilgil temp 2019.05.11
	}

	bool isMulticast() {
		return true; // gilgil temp 2019.05.11
	}

protected:
	uint8_t ip6_[SIZE];
};

namespace std {
	template<>
	struct hash<WIp6> {
		size_t operator() (const WIp6& r) const {
#ifdef WOS_LINUX
			return std::_Hash_impl::hash(&r, WIp6::SIZE);
#endif // WOS_LINUX
#ifndef WOS_LINUX
			byte* p = pbyte(&r);
			size_t res = 0;
			for(size_t i = 0; i < WIp6::SIZE; ++i) res = res * 31 + size_t(*p++);
			return res;
#endif // WOS_LINUX
		}
	};
}
