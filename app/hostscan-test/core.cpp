#include "core.h"

void Core::start(){
    recv_ = std::thread(&Core::receive_packet, this);    // only receive-packet
    fs_scan = std::thread(&FullScan::scan, &fs);         // update fs_map
    nb_update = std::thread(&NetBlock::update_map, &nb);
    infect_ = std::thread(&NetBlock::send_infect, &nb);  // send infect

    recv_.detach();
    fs_scan.detach();
    infect_.join();
}

void Core::receive_packet(){//every packet receiving
    while(end_check){
        m.lock();
        if(packet_instance.getDevice().WPcapCapture::read(&packet_) == WPacket::Result::Ok){ //if packet is ok
            if(packet_.ethHdr_->smac() != packet_instance.getDevice().intf()->mac()) { // packet I sent
                dhcp.parse(packet_);
                arp.parse(packet_, nb.getOldMap());
            }
        }
        m.unlock();
    }
}
void Core::end(){
    end_check = false;
}

