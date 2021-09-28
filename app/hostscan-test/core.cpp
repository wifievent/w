#include "core.h"
#include "parser.h"
void Core::start(){
    DHCPParser dhcp;

    recv_ = new thread(&FullScan::receive_packet,&fs);//only receive-packet
    dhcp_ = new thread(&DHCPParser::parse,&dhcp);//dhcp packet parser
    thread update_(&NetBlock::update_map,&nb);//update FSMap
    thread infect_(&NetBlock::send_infect,&nb);//send infect

    dhcp_->detach();
    recv_->detach();
    update_.join();
    infect_.detach();
}

