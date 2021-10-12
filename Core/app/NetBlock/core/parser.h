#pragma once
#include "netblock.h"
#include "packet.h"

class Parser
{
public:
    Parser() {};
    ~Parser() {};
    FullScan& fs = FullScan::getInstance();
    Packet& packet_instance = Packet::getInstance();
    Host g;
    NetBlock nb;
    virtual bool parse(WPacket& packet) = 0;
};

class DHCPParser: public Parser{
public:
    bool parse(WPacket& packet)override;
};

class ARPParser: public Parser{
public:
    bool parse(WPacket& packet)override;
    void parse(WPacket& packet, std::map<WMac, Host> nb_map);
    void findName();
};
