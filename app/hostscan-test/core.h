#include "parser.h"

class Core
{
private:
    Packet& packet_instance = Packet::instance();
    FullScan& fs = FullScan::instance_fs();
    NetBlock nb;
    DHCPParser dhcp;
    ARPParser arp;
    std::thread *recv_, *fs_update_, *nb_update, *infect_;

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

