#pragma once
#include <mutex>
#include <time.h>
#include "packet.h"
#include "base/db-connect.h"

class FullScan : WObj
{
private:
    class FSMap : public std::map<WMac, Host> {
    public:
        std::mutex m;
    };

    FSMap fsMap;
    Packet& packetInstance = Packet::getInstance();
    int fsTime = 30;
    int sendCountPerIp = 1;
    int sendCountForNextIp = 30;

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
    void updateDB();

    void setHostMap();
    std::map<WMac,Host>& getFsMap(){ return fsMap; }
    void updateHostInfo(WMac mac_, WIp ip_, struct timeval last_);

    void addHost(std::pair<WMac,Host> host);
    bool isConnect(std::string mac);
    void delHost(std::string mac);

    void load(Json::Value& json) override;
    void save(Json::Value& json) override;
};
