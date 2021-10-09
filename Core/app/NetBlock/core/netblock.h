#pragma once
#include "fullscan.h"
#include "pch.h"

class NetBlock
{
private:
    std::map<WMac,Host> nb_map, new_nb_map;
    FullScan& fs_instance = FullScan::getInstance();
    std::mutex m;
public:
    bool end_check = true;
    NetBlock(){};
    ~NetBlock(){};
    enum Week{
        Sunday = 0,
        Monday = 1,
        Tuesday = 2,
        Wednesday = 3,
        Thirsday = 4,
        Friday = 5,
        Saturday = 6,
    };
    void sendInfect();//no sleep
    void sendRecover(Host host);
    void getBlockHostMap();
    void updateMap();//db list update -> 1. db read 2. compare : new-> input list, have to remove -> recover -> per 5minute
    std::map<WMac, Host> getNbMap() { return nb_map; }
};