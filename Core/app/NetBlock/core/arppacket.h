#pragma once
#include "packet.h"
#include "netblock.h"
class ARPPacket : WObj
{
protected:
    Packet& packet_instance = Packet::getInstance();
    WPacket wpacket;
    NetBlock nb;
    int send_time = 3;
public:
    bool end_check = true;
    EthArp packet;
    ARPPacket();
    ~ARPPacket();
    WIp gate_ip;
    WMac gate_mac;//gateway info
    void makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
    void send(int cnt);
    void load(Json::Value& json) override;
    void save(Json::Value& json) override;
};
