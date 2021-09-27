#pragma once
#include "packet.h"
class FullScan
{
private:
    map<WMac,Host> FSMap;
    thread* dhcp;
    Packet& instance = Packet::instance();
    FullScan(){};
    ~FullScan(){};
    WPacket wpacket;
    mutex m; // have to think
public:
    void start();
    void finish();
    void send_ARPpacket(EthArp etharp, int cnt);
    void receive_packet();
    void findName();
    static FullScan& instance_fs(){
        static FullScan fs;
        return fs;
    }
    mutex& getMutex(){return m;}
    WPacket& getWPacket(){return wpacket;}
    void setWPacket(WPacket* wpacket_){wpacket = *wpacket_;}
    map<WMac,Host>& getMap(){return FSMap;}
};
