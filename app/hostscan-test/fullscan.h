#pragma once
#include <mutex>
#include "packet.h"
#include "base/db-connect.h"

class FullScan
{
private:
    class FSMap : public std::map<WMac, Host> {
    public:
        std::mutex m;
    };
    FSMap fs_map;
    Packet& packet_instance = Packet::getInstance();
    FullScan(){};
    ~FullScan(){};
public:
    static FullScan& getInstance(){
        static FullScan fs;
        return fs;
    }
    bool end_check = true;
    void setHostMap();
    void start();
    void scan();
    void updateDB();
    std::map<WMac,Host>& getFsMap(){ return fs_map; }
    void updateHostInfo(WMac mac_, WIp ip_, struct timeval last_);
    void addHost(std::pair<WMac,Host> host);
    // static bool isConnect(std::string mac);
    void delHost(std::string mac);
};
