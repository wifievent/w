#include "parser.h"
class Core
{
private:
    FullScan& fs = FullScan::instance_fs();
    NetBlock nb;
    Packet& instance = Packet::instance();
    DHCPParser dhcp;
    ARPParser arp;
    thread* recv_;
public:
    Core(){};
    ~Core(){};
    void start();//fullscan(connection) -> receive_packet
    void end();//program end
    void receive_packet();
};

