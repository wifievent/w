#include "pch.h"
#include "net/capture/wpcapdevice.h"

class Packet
{
private:
    WPcapDevice pcapdevice;
    Packet();//pcapdevice open
    ~Packet();
public:
    static Packet& getInstance(){
        static Packet packet;
        return packet;
    }
    WPcapDevice& getDevice(){return pcapdevice;}
};
