#include "fullscan.h"
#include "packet.h"
#include "pch.h"

class Parser
{
public:
    Parser() {};
    ~Parser() {};
    FullScan& fs = FullScan::getInstance();
    Packet& instance = Packet::getInstance();
    Host g;
    virtual void parse(WPacket& packet){};
};

class DHCPParser: public Parser{
public:
    void parse(WPacket& packet)override;
};

class ARPParser: public Parser{
private:
    WIntf* intf;
public:
    void parse(WPacket& packet)override;
    void parse(WPacket& packet, std::map<WMac, Host> nb_map);
};
