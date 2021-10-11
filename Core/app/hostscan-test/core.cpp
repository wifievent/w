#include "core.h"
Core::Core()
{
    Json::Value jv;
    if(WJson::loadFromFile("netblock.json",jv)){
        load(jv);
        fs_instance.load(jv);
        nb.load(jv);
        packet.load(jv);
    }
    save(jv);
    fs_instance.save(jv);
    nb.save(jv);
    packet.save(jv);
    WJson::saveToFile("netblock.json",jv);
};

void Core::start()
{
    fs_instance.setHostMap();
    recv_ = std::thread(&Core::receivePacket, this);       // only receive-packet
    fs_scan = std::thread(&FullScan::start, &fs_instance);  // update fs_map
    nb_update = std::thread(&NetBlock::updateMap, &nb);
    infect_ = std::thread(&NetBlock::sendInfect, &nb);
}

void Core::receivePacket()//every packet receiving
{
    WMac my_mac;
    {
        std::lock_guard<std::mutex> lock(packet_instance.m);
        my_mac = packet_instance.intf()->mac();
    }
    while(end_check){
        if(packet_instance.read(&packet_) == WPacket::Result::Ok){ //if packet is ok
            if(packet_.ethHdr_->smac() != my_mac) { // packet I sent
                dhcp.parse(packet_);
                arp.parse(packet_, nb.getNbMap());
            }
        }
    }
}

void Core::stop()
{
    end_check = false;
    fs_instance.end_check = false;
    nb.end_check = false;

    recv_.join();
    fs_scan.join();
    nb_update.join();
    infect_.join();
}

void Core::load(Json::Value& json)
{
    device << json["pcapDevice"];
}

void Core::save(Json::Value& json)
{
    device >> json["pcapDevice"];
}
