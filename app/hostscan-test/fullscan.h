#pragma once
#include "packet.h"
#include "base/db-connect.h"

class FullScan
{
private:
    map<WMac,Host> fs_map;
    thread* dhcp;
    Packet& instance = Packet::instance();
    FullScan(){};
    ~FullScan(){};
    WPacket wpacket;
    mutex m; // have to think
public:
    bool end_check = true;
    void start();
    void finish();
    void send_ARPpacket(EthArp etharp, int cnt);
    void findName(Host* g);
    static FullScan& instance_fs(){
        static FullScan fs;
        return fs;
    }
    mutex& getMutex(){return m;}
    WPacket& getWPacket(){return wpacket;}
    void setWPacket(WPacket* wpacket_){wpacket = *wpacket_;}
    map<WMac,Host>& getMap(){return fs_map;}
    void scan();
    void update_DB();
    void addHost(std::pair<WMac,Host> host);
};
