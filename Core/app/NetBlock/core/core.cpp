#include "core.h"
Core::Core()
{
    Json::Value jv;

    if(WJson::loadFromFile("netblock.json",jv)) {
        packetInstance.load(jv["packet"]);
        fsInstance.load(jv["fs"]);
        nbInstance.load(jv["nb"]);
        packet.load(jv["arppacket"]);
    }

    packetInstance.save(jv["packet"]);
    fsInstance.save(jv["fs"]);
    nbInstance.save(jv["nb"]);
    packet.save(jv["arppacket"]);
    WJson::saveToFile("netblock.json",jv);
};

void Core::start()
{
    fsInstance.setHostMap();
    recv_ = std::thread(&Core::receivePacket, this);       // only receive-packet
    fsScan = std::thread(&FullScan::start, &fsInstance);  // update fs_map
    nbUpdate = std::thread(&NetBlock::updateMap, &nbInstance);
    infect_ = std::thread(&NetBlock::sendInfect, &nbInstance);
}

void Core::receivePacket()//every packet receiving
{
    WMac my_mac;
    {
        std::lock_guard<std::mutex> lock(packetInstance.m);
        my_mac = packetInstance.intf()->mac();
    }
    packetInstance.mtu_ = 1500;
    while(end_check) {
        if(packetInstance.read(&packet_) == WPacket::Result::Ok){ //if packet is ok
            arp.parse(packet_, nbInstance.nbMap);
            dhcp.parse(packet_);
        }
    }
}

void Core::stop()
{
    end_check = false;
    fsInstance.end_check = false;
    nbInstance.end_check = false;

    for(std::map<WMac,Host>::iterator iter = fsInstance.getFsMap().begin(); iter != fsInstance.getFsMap().end(); ++iter) {
        nbInstance.sendRecover(iter->second);
    }

    GTRACE("Before recv_.join");
    recv_.join();
    GTRACE("Before fsScan.join");
    fsScan.join();
    GTRACE("Before nbUpdate.join");
    nbUpdate.join();
    GTRACE("Before infect_.join");
    infect_.join();
}


