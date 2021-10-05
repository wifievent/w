#pragma once
#include "fullscan.h"
#include "packet.h"

class Parser
{
public:
    Parser() {};
    ~Parser() {};
    FullScan& fs = FullScan::getInstance();
    Packet& packet_instance = Packet::getInstance();
    Host g;
    virtual void parse(WPacket& packet){(void)packet;};
};

class DHCPParser: public Parser{
public:
    void parse(WPacket& packet)override;
};

class ARPParser: public Parser{
public:
    void parse(WPacket& packet)override;
    void parse(WPacket& packet, std::map<WMac, Host> nb_map);
    void findName();
};
