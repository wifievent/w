#include "wudphdr.h"

//
// All udpHdr field except udpHdr.uh_sum
// All data buffer(padding)
// ipHdr.ip_src, ipHdr.ip_dst, udpHdrDataLen and IPPROTO_UDP
//
uint16_t WUdpHdr::calcChecksum(WIpHdr* ipHdr, WUdpHdr* udpHdr) {
	uint32_t res = 0;
	int udpHdrDataLen = udpHdr->len();

	// Add udpHdr & data buffer as array of uint16_t
	uint16_t* p = reinterpret_cast<uint16_t*>(udpHdr);
	for (int i = 0; i < udpHdrDataLen / 2; i++) {
		res += htons(*p);
		p++;
	}

	// If length is odd, add last data(padding)
	if ((udpHdrDataLen / 2) * 2 != udpHdrDataLen)
		res += uint32_t(*(reinterpret_cast<uint8_t*>(p)) << 8);

	// Decrease checksum from sum
	res -= udpHdr->sum();

	// Add src address
	uint32_t src = ipHdr->sip();
	res += ((src & 0xFFFF0000) >> 16) + (src & 0x0000FFFF);

	// Add dst address
	uint32_t dst = ipHdr->dip();
	res += ((dst & 0xFFFF0000) >> 16) + (dst & 0x0000FFFF);

	// Add extra information
	res += uint32_t(udpHdrDataLen) + IPPROTO_UDP;

	// Recalculate sum
	while (res >> 16) {
		res = (res & 0xFFFF) + (res >> 16);
	}
	res = ~res;

	return uint16_t(res);
}

WBuf WUdpHdr::parseData(WUdpHdr* udpHdr) {
	WBuf res;
	res.size_ = udpHdr->len() - sizeof(WUdpHdr);
	if (res.size_ > 0)
		res.data_ = reinterpret_cast<u_char*>(udpHdr) + sizeof(WUdpHdr);
	else
		res.data_ = nullptr;
	return res;
}
