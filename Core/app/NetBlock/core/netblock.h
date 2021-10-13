#pragma once
#include "fullscan.h"

class NetBlock : WObj
{
private:
    FullScan& fsInstance = FullScan::getInstance();
    Packet& packet_instance = Packet::getInstance();
    std::mutex m;
    int nbTime = 10000;
    int dbMin = 1;
    int sendRecoverNum = 3;
    NetBlock(){};
    ~NetBlock(){};
public:
    static NetBlock& getInstance(){
        static NetBlock nb;
        return nb;
    }
    enum Week{
        Sunday = 0,
        Monday = 1,
        Tuesday = 2,
        Wednesday = 3,
        Thirsday = 4,
        Friday = 5,
        Saturday = 6,
    };
    std::map<WMac,Host> nbMap, newNbMap;
    int sendInfectNum = 3;
    bool end_check = true;

    void sendInfect();//no sleep
    void sendRecover(Host host);
    void sendRelay(WPacket& packet);


    void getBlockHostMap();
    void updateMap();//db list update -> 1. db read 2. compare : new-> input list, have to remove -> recover -> per 5minute
    std::map<WMac, Host> getNbMap() { return nbMap; }
    void load(Json::Value& json) override;
    void save(Json::Value& json) override;
};
