#include "fullscan.h"
class ARPPacket
{
private:
    EthArp packet;
protected:
    virtual void send(map<WMac,Host> map_){};
    FullScan& fs = FullScan::instance_fs();
public:
    ARPPacket();
    ~ARPPacket();
    WIp mac_ip;
    WMac mac_gate;
    Packet& instance = Packet::instance();
    void makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
    EthArp& getPacket() { return packet; }
};

class Connection : ARPPacket{
public:
    Connection(){};
    ~Connection(){};
    void send(map<WMac,Host> map_) override;
};

class Request : ARPPacket{
public:
    Request(){};
    ~Request(){};
    void send(map<WMac,Host> map_) override;
};

class Recover : ARPPacket{
public:
    Recover(){};
    ~Recover(){};
    void send(map<WMac,Host> map_) override;
};

class Infection : ARPPacket{
public:
    Infection(){};
    ~Infection(){};
    void send(map<WMac,Host> map_) override;
};
