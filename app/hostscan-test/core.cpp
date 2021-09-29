#include "core.h"
void Core::start(){
    recv_ = new thread(&Core::receive_packet,this);//only receive-packet
    thread update_(&NetBlock::update_map,&nb);//update FSMap
    thread infect_(&NetBlock::send_infect,&nb);//send infect

    recv_->detach();
    update_.detach();
    infect_.join();
}

void Core::receive_packet(){//every packet receiving
    WPacket& packet = fs.getWPacket();
    while(check){
        fs.getMutex().lock();
        if(instance.getDevice().WPcapCapture::read(&packet)==WPacket::Result::Ok){ //if packet is ok
            if(packet.ethHdr_->smac()==instance.getDevice().intf()->mac())return; // packet I sent
            fs.setWPacket(&packet);//singleton pattern
            dhcp.parse();
            arp.parse();
        }
        fs.getMutex().unlock();
    }
}
void Core::end(){
    check = false;
}

