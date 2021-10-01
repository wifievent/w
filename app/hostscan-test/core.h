#include "parser.h"
#include "netblock.h"
#include "arppacket.h"

class Core
{
private:
    Packet& packet_instance = Packet::getInstance();
    FullScan& fs = FullScan::getInstance();
    NetBlock nb;
    DHCPParser dhcp;
    ARPParser arp;
    std::thread recv_, fs_scan, nb_update, infect_;

    WPacket packet_;
    std::mutex m;

public:
    bool end_check = true;
    Core(){};
    ~Core(){};
    void start();//fullscan(connection) -> receive_packet
    void end();//program end
    void receive_packet();
};
