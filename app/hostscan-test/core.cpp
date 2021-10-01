#include "core.h"

void Core::start(){
    recv_ = std::thread(&Core::receive_packet, this);    // only receive-packet
    fs_scan = std::thread(&FullScan::start, &fs);         // update fs_map
    nb_update = std::thread(&NetBlock::update_map, &nb);
    infect_ = std::thread(&NetBlock::sendInfect, &nb);  // send infect
}

void Core::receive_packet(){//every packet receiving
    while(end_check){
        int result = packet_instance.getDevice().read(&packet_);

        if(result == WPacket::Result::Ok){ //if packet is ok
            if(packet_.ethHdr_->smac() != packet_instance.getDevice().intf()->mac()) { // packet I sent
                dhcp.parse(packet_);
                arp.parse(packet_, nb.getNbMap());
            }
        }
    }
}
void Core::stop(){
    end_check = false;
    fs.end_check = false;
    nb.end_check = false;
    recv_.join();
    fs_scan.join();
    nb_update.join();
    infect_.join();
}
