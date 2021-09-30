#include "netblock.h"

class ARPPacket
{
private:
    EthArp packet;
protected:
    virtual void send(){};
    FullScan& fs = FullScan::instance_fs();
    NetBlock nb;
public:
    bool end_check = true;
    ARPPacket();
    ~ARPPacket();
    WIp mac_ip;
    WIntf* intf_g;//gateway info
    Packet& instance = Packet::instance();
    void makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
    EthArp& getPacket() { return packet; }
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
public:
    Infection(){};
    ~Infection(){};
    void send() override;
};
