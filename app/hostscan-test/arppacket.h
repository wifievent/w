#pragma once
#include "packet.h"
#include "pch.h"

class ARPPacket
{
protected:
    Packet& instance = Packet::getInstance();
    WPacket wpacket;
    virtual void send(){};
public:
    bool end_check = true;
    EthArp packet;
    ARPPacket();
    ~ARPPacket();
    WIp mac_ip;
    WIntf* intf_g;//gateway info
    void makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
    EthArp& getPacket() { return packet; }
    void send(int cnt);
};
