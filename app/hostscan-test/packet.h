#include "pch.h"
class Packet
{
private:
    WPcapDevice pcapdevice;
    Packet();//pcapdevice open
    ~Packet();
public:
    static Packet& instance(){
        static Packet packet;
        return packet;
    }
    WPcapDevice& getDevice(){return pcapdevice;}
    void send(WPacket p);
    void receive(WPacket* p);
};
