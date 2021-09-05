#include "wpacketdbg.h"
#include <iostream>

std::string dump(unsigned char* data, size_t size) {
	std::string raw;
	std::string hexa;
	if (size > 16) size = 16;
	while (size > 0) {
		char ch = char(*data);
		if (isprint(u_char(ch)))
			raw += ch;
		else
			raw += '.';

		char ch1 = (ch & 0xF0) >> 4;
		if (ch1 >= 10)
			ch1 += 'A' - 10;
		else
			ch1 += '0';
		char ch2 = (ch & 0x0F);
		if (ch2 >= 10)
			ch2 += 'A' - 10;
		else
			ch2 += '0';
		hexa += ch1;
		hexa += ch2;
		hexa += ' ';

		data++;
		size--;
	}
	return raw + " " + hexa;
}

void WPacketDbg::debug(WPacket* packet) {
	char temp[BUFSIZ];
	std::string msg;

	WEthHdr* ethHdr = packet->ethHdr_;
	if (ethHdr != nullptr) {
		sprintf(temp, " eth %s>%s", std::string(ethHdr->smac()).data(), std::string(ethHdr->dmac()).data());
		msg += temp;

		WIpHdr* ipHdr = packet->ipHdr_;
		if (ipHdr != nullptr) {
			sprintf(temp, " ip %s>%s", std::string(ipHdr->sip()).data(), std::string(ipHdr->dip()).data());
			msg += temp;

			WTcpHdr* tcpHdr = packet->tcpHdr_;
			if (tcpHdr != nullptr) {
				sprintf(temp, " tcp %d>%d", tcpHdr->sport(), tcpHdr->dport());
				msg += temp;

				WBuf tcpData = packet->tcpData_;
				if (tcpData.valid())
					msg += " " + dump(tcpData.data_, tcpData.size_);
			}

			WUdpHdr* udpHdr = packet->udpHdr_;
			if (udpHdr != nullptr) {
				sprintf(temp, " udp %d>%d", udpHdr->sport(), udpHdr->dport());
				msg += temp;

				WBuf udpData = packet->udpData_;
				if (udpData.valid())
					msg += " " + dump(udpData.data_, udpData.size_);
			}

			WIcmpHdr* icmpHdr = packet->icmpHdr_;
			if (icmpHdr != nullptr)
				msg += " icmp";
		}

		std::cout << msg << std::endl;
	}
}
