#include "parser.h"

void DHCPParser::parse(WPacket& packet)
{
    if(packet.ethHdr_->type() != WEthHdr::Ip4) { return; }  // Is ip4 packet?
    if(packet.ipHdr_->p() != WIpHdr::Udp) { return; }       // Is udp packet?
    if(packet.udpHdr_->sport() != 67 && packet.udpHdr_->dport() != 67) { return; }  // Is dhcp packet?
    //  sport와 dport 둘 다가 67 이면 안되는 것? 아니면 둘 다 67이어야되는 것?
    if(!packet.ethHdr_->dmac_.isBroadcast()) { return; }    // Is DHCP Request?

    packet.dhcpHdr_ = (WDhcpHdr*)packet.udpData_.data_;
    gtrace("<Dhcp>");

    g.mac_ = packet.dhcpHdr_->clientMac_;//get mac
    g.active = true;//get active = true
    gtrace("%s",std::string(g.mac_).data());

    for(WDhcpHdr::Option* opt = packet.dhcpHdr_->first(); opt != nullptr; opt = opt->next())
    {
        if(opt->type_ == WDhcpHdr::RequestedIpAddress)//get ip
        {
            char* buf = (char*)malloc(sizeof(char) * 4);
            for(int i = 0; i < opt->len_ - 1; ++i)
            {
                sprintf(buf + 4 * i, "%03d.", *(&opt->len_ + 1 + i));
            }
            sprintf(buf + 12, "%03d", *(&opt->len_ + 4));
            g.ip_ = WIp(std::string(buf));
            gtrace("%s", std::string(g.ip_).data());
        }
        else if(opt->type_ == 12)//get name
        {
            g.name = (char*)malloc(sizeof(char) * opt->len_);
            for(int i = 0; i < opt->len_; ++i)
            {
                g.name[i] = *(&opt->len_ + 1 + i);
            }
        }
    }

    fs.addHost(pair<WMac,Host>(g.mac_, g));//insert into fs_map
}

void ARPParser::parse(WPacket& packet) //arp packet parsing
{
    intf = instance.getDevice().intf();

    if(packet.ethHdr_->type() != WEthHdr::Arp) {return;}
    if(packet.ethHdr_->dmac_ != intf->mac()) {return;}

    WIp mask = intf->mask();

    if((packet.arpHdr_->sip() & mask) == (intf->gateway() & mask))
    {
        g.mac_ = packet.ethHdr_->smac();//get mac
        g.ip_ = packet.arpHdr_->sip(); //get ip
        g.active = true; //get active
        gtrace("<full scan>");
        gtrace("%s",string(g.mac_).data());
        gtrace("%s",string(g.ip_).data());
        std::map<WMac, Host>::iterator iter = fs.getMap().find(g.mac_);
        if(iter != fs.getMap().end()) {
            iter->second.ip_ = g.ip_;
            iter->second.active = true;
        }
        else {
            fs.findName(&g);
            fs.addHost(std::pair<WMac,Host>(g.mac_, g)); //insert into fs_map
        }
    }
}

void ARPParser::parse(WPacket& packet, std::map<WMac, Host> nb_map) {
    parse(packet);

    if(packet.arpHdr_->op()==packet.arpHdr_->Request){//request
        //infection
        std::map<WMac,Host>::iterator iter;
        for(iter = nb_map.begin(); iter != nb_map.end(); ++iter) {
            if(packet.arpHdr_->tip() == iter->second.ip_) {
                break;
            }
        }
        if(packet.arpHdr_->sip() == arppacket.intf_g->ip() && iter != nb_map.end()){//gateway
            arppacket.makeArppacket(arppacket.intf_g->mac(),intf->mac(),arppacket.intf_g->mac(),arppacket.intf_g->ip(),packet.arpHdr_->tip());
        }else if(nb_map.find(g.mac_)!=nb_map.end()){
            arppacket.makeArppacket(g.mac_,intf->mac(),g.mac_,g.ip_,arppacket.intf_g->gateway());
        }
        else {
            return;
        }
        arppacket.getPacket().arp.op_ = htons(WArpHdr::Reply);
        fs.send_ARPpacket(arppacket.getPacket(),3);
    }
}