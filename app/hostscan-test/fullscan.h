#pragma once
#include "arppacket.h"
#include "base/db-connect.h"

class FullScan
{
private:
    map<WMac,Host> fs_map;
    thread* dhcp;
    FullScan(){};
    ~FullScan(){};
    Connection conn;
    std::thread conn_th;
public:
    static FullScan& getInstance(){
        static FullScan fs;
        return fs;
    }
    bool end_check = true;
    void start();
    void finish();
    void findName(Host* g);
    map<WMac,Host>& getMap(){return fs_map;}
    void scan();
    void update_DB();
    void addHost(std::pair<WMac,Host> host);
    bool isConnect(std::string mac);
    void delHost(std::string mac);
};
