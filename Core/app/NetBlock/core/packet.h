#pragma once
#include "pch.h"
#include "net/capture/wpcapdevice.h"

struct Packet : WPcapDevice
{
private:
    Packet();//pcapdevice open
    ~Packet();
public:
    static Packet& getInstance() {
        static Packet packet;
        return packet;
    }
    std::thread t;
    std::mutex m;
    void load(Json::Value& json) override;
    void save(Json::Value& json) override;
};
