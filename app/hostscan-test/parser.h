#pragma once
#include "fullscan.h"
#include "packet.h"

class Parser
{
public:
    Parser() {};
    ~Parser() {};
    FullScan& fs = FullScan::getInstance();
    Packet& instance = Packet::getInstance();
    Host g;
    virtual int parse(WPacket& packet){(void)packet;};
};

class DHCPParser: public Parser{
public:
    int parse(WPacket& packet)override;
};

class ARPParser: public Parser{
    WIntf* intf;
public:
    int parse(WPacket& packet)override;
    void parse(WPacket& packet, std::map<WMac, Host> nb_map);
};
