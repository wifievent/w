#include "parser.h"
#include "netblock.h"
#include "arppacket.h"

class Core : WObj
{
private:
    Packet& packet_instance = Packet::getInstance();
    FullScan& fs_instance = FullScan::getInstance();
    NetBlock nb;
    DHCPParser dhcp;
    ARPParser arp;
    ARPPacket packet;
    std::thread recv_, fs_scan, nb_update, infect_;
    WPcapDevice device;
    WPacket packet_;

public:
    bool end_check = true;
    Core();
    ~Core(){};
    void start();//fullscan(connection) -> receive_packet
    void stop();//program end
    void receivePacket();
    void load(Json::Value& json) override;
    void save(Json::Value& json) override;
};
