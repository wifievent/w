#include "packet.h"
Packet::Packet()
{
    open();
}
Packet::~Packet(){
    close();
}
void Packet::load(Json::Value& json)
{
    json["pcapDevice"] >> this->intfName_;
}

void Packet::save(Json::Value& json)
{
    json["pcapDevice"] << this->intfName_;
}
