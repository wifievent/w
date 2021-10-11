#include "packet.h"
Packet::Packet()
{
    open();
}
Packet::~Packet(){
    close();
}
