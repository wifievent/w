#pragma once
#include "packet.h"
#include "fullscan.h"
#include "pch.h"

class ARPPacket
{
protected:
    Packet& instance = Packet::getInstance();
    FullScan& fs = FullScan::getInstance();
    EthArp packet;
    WPacket wpacket;
    virtual void send(){};
public:
    bool end_check = true;
    ARPPacket();
    ~ARPPacket();
    WIp mac_ip;
    WIntf* intf_g;//gateway info
    void makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
    EthArp& getPacket() { return packet; }
    void send(EthArp p, int cnt);
};

class Connection : ARPPacket{
public:
    Connection(){};
    ~Connection(){};
    void send() override;

};

class Request : ARPPacket{//remove?
public:
    Request(){};
    ~Request(){};
    void send() override;
};

class Recover : ARPPacket{
public:
    Recover(){};
    ~Recover(){};
    void send() override;
};

class Infection : ARPPacket{
private:
    NetBlock nb;
public:
    Infection(){};
    ~Infection(){};
    void send() override;
};
