#pragma once

#include <pcap.h>
#include "net/pdu/wpdu.h"
#include "net/pdu/wethhdr.h"
#include "net/pdu/warphdr.h"
#include "net/pdu/wiphdr.h"
#include "net/pdu/wip6hdr.h"
#include "net/pdu/wtcphdr.h"
#include "net/pdu/wudphdr.h"
#include "net/pdu/wicmphdr.h"
#include "net/pdu/wdhcphdr.h"
struct WPacket {
    enum Result {
        Eof = -2, // read
        Fail = -1, // read write
        None = 0, // read
        Ok = 1, // read write
    };

public:
    WPacket() { clear(); }
    WPacket(const WPacket& r);
    virtual ~WPacket() {}

    //
    // sniffing
    //
    struct timeval ts_;
    WBuf buf_;

    //
    // header
    //
    WEthHdr* ethHdr_{nullptr};
    WArpHdr* arpHdr_{nullptr};

    WIpHdr* ipHdr_{nullptr};
    WIp6Hdr* ip6Hdr_{nullptr};

    WTcpHdr* tcpHdr_{nullptr};
    WUdpHdr* udpHdr_{nullptr};
    WIcmpHdr* icmpHdr_{nullptr};
    WDhcpHdr* dhcpHdr_{nullptr};

    WBuf tcpData_;
    WBuf udpData_;

    //
    // constant
    //
    static constexpr int MaxBufSize = 32768;
    static constexpr int MtuSize = 1500;

public:
    void clear() {
        ts_.tv_sec = 0;
        ts_.tv_usec = 0;
        buf_.clear();
        ethHdr_ = nullptr;
        arpHdr_ = nullptr;
        ipHdr_ = nullptr;
        ip6Hdr_ = nullptr;
        tcpHdr_ = nullptr;
        udpHdr_ = nullptr;
        icmpHdr_ = nullptr;
        dhcpHdr_ = nullptr;
        tcpData_.clear();
        udpData_.clear();
    }

    void parse();
};
typedef WPacket *PPacket;
