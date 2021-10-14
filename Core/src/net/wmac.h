#pragma once

#include <cstdint>
#include <cstring>
#include <string>

struct WMac final {
	static constexpr int SIZE = 6;

	// constructor
	WMac() {}
	WMac(const WMac& r) { memcpy(this->mac_, r.mac_, SIZE); }
	WMac(const uint8_t* r) { memcpy(this->mac_, r, SIZE); }
	WMac(const std::string& r);

	// assign operator
	WMac& operator = (const WMac& r) { memcpy(this->mac_, r.mac_, SIZE); return *this; }

	// casting operator
	explicit operator uint8_t*() const { return const_cast<uint8_t*>(mac_); }
	explicit operator std::string() const;

	// comparison operator
	bool operator == (const WMac& r) const { return memcmp(mac_, r.mac_, SIZE) == 0; }
	bool operator != (const WMac& r) const { return memcmp(mac_, r.mac_, SIZE) != 0; }
	bool operator < (const WMac& r) const { return memcmp(mac_, r.mac_, SIZE) < 0; }
	bool operator > (const WMac& r) const { return memcmp(mac_, r.mac_, SIZE) > 0; }
	bool operator <= (const WMac& r) const { return memcmp(mac_, r.mac_, SIZE) <= 0; }
	bool operator >= (const WMac& r) const { return memcmp(mac_, r.mac_, SIZE) >= 0; }
	bool operator == (const uint8_t* r) const { return memcmp(mac_, r, SIZE) == 0; }

	void clear() {
		*this = nullMac();
	}

	bool isNull() const {
		return *this == nullMac();
	}

	bool isBroadcast() const { // FF:FF:FF:FF:FF:FF
		return *this == broadcastMac();
	}

	bool isMulticast() const { // 01:00:5E:0*
		return mac_[0] == 0x01 && mac_[1] == 0x00 && mac_[2] == 0x5E && (mac_[3] & 0x80) == 0x00;
	}
    void print(){
        for(int i =0; i<5; i++)
            printf("%x:",mac_[i]);
        printf("%x\n",mac_[5]);
    }
	static WMac randomMac();
	static WMac& nullMac();
	static WMac& broadcastMac();

protected:
	uint8_t mac_[SIZE];
};
typedef WMac *PMac;

namespace std {
	template<>
	struct hash<WMac> {
		size_t operator() (const WMac& r) const {
			return std::_Hash_impl::hash(&r, WMac::SIZE);
		}
	};
}
