#include "arppacket.h"

ARPPacket::ARPPacket()
{
};

ARPPacket::~ARPPacket(){};

void ARPPacket::makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip)
{
    packet.eth.dmac_ = dmac;
    packet.eth.smac_ = smac;
    packet.eth.type_ = htons(WEthHdr::Arp);

    packet.arp.hrd_ = htons(WArpHdr::ETHER);
    packet.arp.pro_ = htons(WEthHdr::Ip4);
    packet.arp.hln_ = WMac::SIZE;
    packet.arp.pln_ = WIp::SIZE;
    packet.arp.op_ = htons(WArpHdr::Request);
    packet.arp.smac_ = smac;
    packet.arp.sip_ = htonl(sip);
    packet.arp.tmac_ = tmac;
    packet.arp.tip_ = htonl(tip);
}

void ARPPacket::send(int cnt)
{
    wpacket.buf_.data_ = reinterpret_cast<byte*>(&packet);
    wpacket.buf_.size_ = sizeof(EthArp);
    for(int i =0; i<cnt; i++) {
        packet_instance.write(wpacket.buf_);
        std::this_thread::sleep_for(std::chrono::milliseconds(send_time));
    }
}

void ARPPacket::load(Json::Value& json) {
    json["send_time"] >> send_time;
}

void ARPPacket::save(Json::Value& json) {
    json["send_time"] << send_time;
}
