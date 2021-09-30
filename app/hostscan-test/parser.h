#include "arppacket.h"

class Parser
{
public:
    Parser() {};
    ~Parser() {};
    FullScan& fs = FullScan::instance_fs();
    Packet& instance = Packet::instance();
    ARPPacket arppacket;
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
