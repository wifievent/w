#include "parser.h"
#include "arppacket.h"

int DHCPParser::parse(WPacket& packet)
{
    if(packet.ethHdr_->type() != WEthHdr::Ip4) { return 0; }  // Is ip4 packet?
    if(packet.ipHdr_->p() != WIpHdr::Udp) { return 0; }       // Is udp packet?
    if(packet.udpHdr_->sport() != 67 && packet.udpHdr_->dport() != 67) { return 0; }  // Is dhcp packet?
    if(!packet.ethHdr_->dmac_.isBroadcast()) { return 0; }    // Is DHCP Request?

    packet.dhcpHdr_ = (WDhcpHdr*)packet.udpData_.data_;
    gtrace("<Dhcp>");

    g.mac_ = packet.dhcpHdr_->clientMac_;//get mac
    gettimeofday(&g.last,NULL);//get is_connect = true
    gtrace("%s",std::string(g.mac_).data());

    for(WDhcpHdr::Option* opt = packet.dhcpHdr_->first(); opt != nullptr; opt = opt->next()){
        if(opt->type_ == WDhcpHdr::RequestedIpAddress){//get ip
            char* buf = (char*)malloc(sizeof(char) * 4);
            for(int i = 0; i < opt->len_ - 1; ++i){
                sprintf(buf + 4 * i, "%03d.", *(&opt->len_ + 1 + i));
            }
            sprintf(buf + 12, "%03d", *(&opt->len_ + 4));

            WIp mask = intf->mask();
            //out of bound
            if((WIp(std::string(buf)) & mask) != (intf->gateway() & mask)){ return 0; }

            g.ip_ = WIp(std::string(buf));
            gtrace("%s", std::string(g.ip_).data());
        }
        else if(opt->type_ == 12){//get name
            g.name = (char*)malloc(sizeof(char) * opt->len_);
            for(int i = 0; i < opt->len_; ++i){
                g.name[i] = *(&opt->len_ + 1 + i);
            }
        }
    }

    fs.addHost(pair<WMac,Host>(g.mac_, g));//insert into fs_map
    return 1;
}

int ARPParser::parse(WPacket& packet) //arp packet parsing
{
    if(packet.ethHdr_->type() != WEthHdr::Arp) { return 0; }
    if(packet.ethHdr_->dmac_ != intf->mac()) { return 0; }
    if((packet.arpHdr_->sip() == intf->gateway())){ return 0; } //if gateway

    WIp mask = intf->mask();

    gtrace("<full scan>");
    if((packet.arpHdr_->sip() & mask) == (intf->gateway() & mask)) {
        g.mac_ = packet.ethHdr_->smac();//get mac
        g.ip_ = packet.arpHdr_->sip(); //get ip
        gettimeofday(&g.last,NULL);//get is_connect

        gtrace("%s",string(g.mac_).data());
        gtrace("%s",string(g.ip_).data());

        map<WMac, Host>::iterator iter = fs.getFsMap().find(g.mac_);
        if(iter != fs.getFsMap().end()) {//found
            iter->second.ip_ = g.ip_;
            gettimeofday(&iter->second.last,NULL);//get is_connect
        }
        else {//not found
            g.name = string(g.ip_).data();
            gtrace("name = %s", g.name.data());
            //fs.findName(&g); -> error exist
            fs.addHost(pair<WMac,Host>(g.mac_, g)); //insert into fs_map
        }
    }
    return 1;
}

void ARPParser::parse(WPacket& packet, map<WMac, Host> nb_map)
{
    if(!parse(packet)) return;

    ARPPacket arp_packet;
    int tmp;
    if(packet.arpHdr_->op()==packet.arpHdr_->Request){//request
        //infection
        map<WMac,Host>::iterator iter;
        tmp = 0;
        for(iter = nb_map.begin(); iter != nb_map.end(); ++iter) {
            if(packet.arpHdr_->tip() == iter->second.ip_ && packet.arpHdr_->sip_ == arp_packet.intf_g->ip()) {//packet from gateway
                tmp = 1;
                break;
            }
            else if(packet.arpHdr_->sip() == iter->second.ip_ && packet.arpHdr_->tip() == arp_packet.intf_g->ip()) { //packet from infected device
                tmp = 1;
                break;
            }
        }
        if(iter != nb_map.end() && tmp) {
            //infect gateway
            arp_packet.makeArppacket(arp_packet.intf_g->mac(), intf->mac(), arp_packet.intf_g->mac(), arp_packet.intf_g->ip(), packet.arpHdr_->tip());
            arp_packet.packet.arp.op_ = htons(WArpHdr::Reply);
            arp_packet.send(3);

            //infect device
            arp_packet.makeArppacket(g.mac_, intf->mac(), g.mac_, g.ip_, arp_packet.intf_g->ip());
            arp_packet.packet.arp.op_ = htons(WArpHdr::Reply);
            arp_packet.send(3);
        }
    }
}
