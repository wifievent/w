#include "core.h"
Core::Core()
{
    Json::Value jv;

    if(WJson::loadFromFile("netblock.json",jv)){

        load(jv["core"]);
        fsInstance.load(jv["fs"]);
        nb.load(jv["nb"]);
        packet.load(jv["packet"]);
    }

    save(jv["core"]);
    fsInstance.save(jv["fs"]);
    nb.save(jv["nb"]);
    packet.save(jv["packet"]);
    WJson::saveToFile("netblock.json",jv);
};

void Core::start()
{
    fsInstance.setHostMap();
    recv_ = std::thread(&Core::receivePacket, this);       // only receive-packet
    fsScan = std::thread(&FullScan::start, &fsInstance);  // update fs_map
    nbUpdate = std::thread(&NetBlock::updateMap, &nb);
    infect_ = std::thread(&NetBlock::sendInfect, &nb);
}

void Core::receivePacket()//every packet receiving
{
    WMac my_mac;
    {
        std::lock_guard<std::mutex> lock(packetInstance.m);
        my_mac = packetInstance.intf()->mac();
    }
    while(end_check) {
        if(packetInstance.read(&packet_) == WPacket::Result::Ok){ //if packet is ok
            if(packet_.ethHdr_->smac() != my_mac) { // packet I sent
                dhcp.parse(packet_);
                arp.parse(packet_, nb.getNbMap());
                nb.sendRelay(packet_);
            }
        }
    }
}

void Core::stop()
{
    end_check = false;
    fsInstance.end_check = false;
    nb.end_check = false;
    for(std::map<WMac,Host>::iterator iter = fsInstance.getFsMap().begin(); iter != fsInstance.getFsMap().end(); ++iter) {
        nb.sendRecover(iter->second);
    }
    recv_.join();
    fsScan.join();
    nbUpdate.join();
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
