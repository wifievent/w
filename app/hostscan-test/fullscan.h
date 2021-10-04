#pragma once
#include <mutex>
#include "packet.h"
#include "base/db-connect.h"

class FullScan
{
private:
    class FSMap: public map<WMac,Host>{
    public:
        mutex m;
        void lock(){m.lock();}
        void unlock(){m.unlock();}
    };
    FSMap fs_map;
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
    map<WMac,Host>& getFsMap(){ return fs_map; }
    void addHost(pair<WMac,Host> host);
    bool isConnect(string mac);
    void delHost(string mac);
};
