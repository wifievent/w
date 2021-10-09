#include "wwritable.h"
#include <thread>

WPacket::Result WWritable::writeMtuSplit(WPacket* packet, size_t mtu) {
	//qDebug() << packet->buf_.size_; // gilgil temp 2021.07.10

	WEthHdr* ethHdr = packet->ethHdr_;
	if (ethHdr == nullptr) {
		GTRACE("ethHdr is null");
		return WPacket::Fail;
	}

	WIpHdr* ipHdr = packet->ipHdr_;
	if (ipHdr == nullptr) {
		GTRACE("ipHdr is null");
		return WPacket::Fail;
	}

	WTcpHdr* tcpHdr = packet->tcpHdr_;
	if (tcpHdr == nullptr) {
		GTRACE("tcpHdr is null");
		return WPacket::Fail;
	}

	WBuf tcpData = packet->tcpData_;
	if (tcpData.data_ == nullptr) {
		GTRACE("tcpData is null");
		return WPacket::Fail;
	}

	WBuf backupBuf = packet->buf_;
	if (tempBuf_.data_ == nullptr) {
		tempBuf_.size_ = packet->buf_.size_;
		tempBuf_.data_ = new byte[tempBuf_.size_];
	} else {
		if (tempBuf_.size_ < packet->buf_.size_) {
			delete[] tempBuf_.data_;
			tempBuf_.size_ = packet->buf_.size_;
			tempBuf_.data_ = new byte[tempBuf_.size_];
		}
	}
	memcpy(tempBuf_.data_, packet->buf_.data_, packet->buf_.size_);

	ipHdr->len_ = ntohs(mtu);
	ipHdr->sum_ = htons(WIpHdr::calcChecksum(ipHdr));

	byte* tcpDataData = tcpData.data_;

	size_t ipTcpHdrSize = (ipHdr->hl() + tcpHdr->off()) * 4;
	size_t totalTcpDataSize = packet->buf_.size_ - (sizeof(WEthHdr) + ipTcpHdrSize);
	while (true) {
		if (ipTcpHdrSize + totalTcpDataSize <= mtu) break;

		packet->buf_.size_ = sizeof(WEthHdr) + mtu;
		size_t onceTcpDataSize = mtu - ipTcpHdrSize;
		//qDebug() << "onceTcpDataSize =" << onceTcpDataSize; // gilgil temp 2021.07.10
		tcpHdr->sum_ = htons(WTcpHdr::calcChecksum(ipHdr, tcpHdr));
		write(packet->buf_);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		tcpHdr->seq_ = htonl(tcpHdr->seq() + onceTcpDataSize); // next seq
		totalTcpDataSize -= onceTcpDataSize;
		memcpy(tcpDataData, tcpDataData + onceTcpDataSize, totalTcpDataSize); // next data
	}
	//qDebug() << "lastTcpDataSize =" << totalTcpDataSize; // gilgil temp 2021.07.10
	ipHdr->len_ = ntohs(ipTcpHdrSize + totalTcpDataSize);
	ipHdr->sum_ = htons(WIpHdr::calcChecksum(ipHdr));
	tcpHdr->sum_ = htons(WTcpHdr::calcChecksum(ipHdr, tcpHdr));
	packet->buf_.size_ = sizeof(WEthHdr) + ipTcpHdrSize + totalTcpDataSize;
	WPacket::Result res = write(packet->buf_); // gilgil temp 2021.07.10

	packet->buf_ = backupBuf;
	memcpy(packet->buf_.data_, tempBuf_.data_, packet->buf_.size_);
	return res;
}
