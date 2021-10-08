#pragma once
#include "packet.h"

class ARPPacket
{
protected:
    Packet& packet_instance = Packet::getInstance();
    WPacket wpacket;
public:
    bool end_check = true;
    EthArp packet;
    ARPPacket();
    ~ARPPacket();
    WIp mac_ip;
    WIntf* intf_g;//gateway info
    void makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
    void send(int cnt);
};
