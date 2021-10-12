#include "parser.h"
#include "netblock.h"
#include "arppacket.h"

class Core : WObj
{
private:
    Packet& packetInstance = Packet::getInstance();
    FullScan& fsInstance = FullScan::getInstance();
    NetBlock nb;
    DHCPParser dhcp;
    ARPParser arp;
    ARPPacket packet;
    std::thread recv_, fsScan, nbUpdate, infect_;
    WPcapDevice device;
    WPacket packet_;

public:
    bool end_check = true;
    Core();
    ~Core(){};
    void start();//fullscan(connection) -> receive_packet
    void stop();//program end
    void receivePacket();
};
