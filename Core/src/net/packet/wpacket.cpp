#include "wpacket.h"

void WPacket::parse() {
	ethHdr_ = PEthHdr(buf_.data_);
	switch (ethHdr_->type()) {
		case WEthHdr::Ip4:
		case WEthHdr::Ip6: {
			byte* p = buf_.data_ + sizeof(WEthHdr);
			uint8_t proto;
			switch (*p & 0xF0) {
				case 0x40: // version 4
					ipHdr_ = PIpHdr(p);
					proto = ipHdr_->p();
					p += ipHdr_->hl() * 4;
					break;
				case 0x60: // version 6
					ip6Hdr_= PIp6Hdr(p);
					proto = ip6Hdr_->nh();
					p += sizeof(WIp6Hdr);
					break;
				default:
					GTRACE("invalid ip header version 0x%x", uint8_t(*p));
					proto = 0; // unknown
					break;
			}

			switch (proto) {
				case WIpHdr::Tcp: // Tcp
					tcpHdr_ = PTcpHdr(p);
					// p += tcpHdr_->off() * 4;
					tcpData_ = WTcpHdr::parseData(ipHdr_, tcpHdr_);
					break;
				case WIpHdr::Udp: // Udp
					udpHdr_ = PUdpHdr(p);
					// p += sizeof(GUdpHdr);
					udpData_ = WUdpHdr::parseData(udpHdr_);
					break;
				case WIpHdr::Icmp: // Icmp
					icmpHdr_ = PIcmpHdr(p);
					// p += sizeof(GIcmpHdr);
					break;
				default:
					break;
			}
			break;
		}
		case WEthHdr::Arp:
			arpHdr_ = PArpHdr(buf_.data_ + sizeof(WEthHdr));
			break;
	}
}
