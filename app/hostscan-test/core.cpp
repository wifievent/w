#include "core.h"

void Core::start(){
    fs.getFsMap().clear();
    recv_ = std::thread(&Core::receive_packet, this);       // only receive-packet
    fs_scan = std::thread(&FullScan::start, &fs_instance);  // update fs_map
    nb_update = std::thread(&NetBlock::update_map, &nb);
    infect_ = std::thread(&NetBlock::sendInfect, &nb);      // send infect
    // receive_packet();
}

void Core::receive_packet(){//every packet receiving
    while(end_check){
        int result;
        {
            std::lock_guard<std::mutex> lock(packet_instance.m);
            result = packet_instance.read(&packet_);
        }

        GTRACE("result: %d", result);

        if(result == WPacket::Result::Ok){ //if packet is ok
            if(packet_.ethHdr_->smac() != packet_instance.intf()->mac()) { // packet I sent
                dhcp.parse(packet_);
                arp.parse(packet_, nb.getNbMap());
            }
        }
    }
}

void Core::stop(){
    end_check = false;
    fs_instance.end_check = false;
    nb.end_check = false;

    recv_.join();
    fs_scan.join();
    nb_update.join();
    infect_.join();
}
