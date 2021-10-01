#pragma once
#include <mutex>
#include "packet.h"
#include "base/db-connect.h"

class FullScan
{
private:
    std::map<WMac, Host> fs_map;
    std::mutex m;
    Packet& instance = Packet::getInstance();
    FullScan(){};
    ~FullScan(){};
public:
    static FullScan& getInstance(){
        static FullScan fs;
        return fs;
    }
    bool end_check = true;
    void start();
    void scan();
    void update_DB();
    void finish();
    void findName(Host* g);
    std::map<WMac,Host>& getFsMap(){ return fs_map; }
    void addHost(std::pair<WMac,Host> host);
    static bool isConnect(std::string mac);
    void delHost(std::string mac);
};
