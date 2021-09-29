#include "parser.h"

void DHCPParser::parse()
{
    Parser parser;
    while(1){
        WPacket& packet = fs.getWPacket();//get packet -> singleton pattern
        if(packet.ethHdr_->type()!=WEthHdr::Ip4)continue;//is ip4 packet?
        if(packet.ipHdr_->p_!=WIpHdr::Udp)continue;//is udp packet?
        if(packet.udpHdr_->sport()!=67&&packet.udpHdr_->dport()!=67)continue;//Is dhcp packet?
        if(!packet.ethHdr_->dmac_.isBroadcast())continue; // Is DHCP Request?

        packet.dhcpHdr_ = (WDhcpHdr*)packet.udpData_.data_;
        gtrace("<Dhcp>");

        Host g;
        g.mac_ = packet.dhcpHdr_->clientMac_;//get mac
        g.active = 1;//get active = 1
        gtrace("%s",std::string(g.mac_).data());

        for(WDhcpHdr::Option* opt = packet.dhcpHdr_->first(); opt!=nullptr; opt=opt->next())
        {
            if(opt->type_ == WDhcpHdr::RequestedIpAddress)//get ip
            {
                char* buf = (char*)malloc(sizeof(char)*4);
                for(int i = 0; i<opt->len_-1; i++)
                    sprintf(buf+4*i,"%03d.",*(&opt->len_+1+i));
                sprintf(buf+12,"%03d",*(&opt->len_+4));

                string str(buf);
                g.ip_ = WIp(str);
                gtrace("%s",std::string(g.ip_).data());
            }
            else if(opt->type_ == 12)//get name
            {
                g.name = (char*)malloc(sizeof(char)*opt->len_);
                for(int i = 0; i<opt->len_;i++)
                    g.name[i] = *(&opt->len_+1+i);
            }
        }

        //using mutex
        fs.getMutex().lock();
        fs.getMap().insert(pair<WMac,Host>(g.mac_, g));//insert into FSMap
        fs.getMutex().unlock();
    }
}

void ARPParser::parse() //arp packet parsing
{
    Parser parser;
    WIntf* intf = instance.getDevice().intf();
    time_t start, end;
    start = time(NULL);//time measuring

    while(1) {
        WPacket& packet = fs.getWPacket();
        if(packet.ethHdr_->type()!=WEthHdr::Arp)continue;
        if(packet.ethHdr_->dmac_!=intf->mac()||packet.arpHdr_->op()!=packet.arpHdr_->Reply)continue;

        WIp mask = intf->mask();
        Host g;

        if((packet.arpHdr_->sip()&mask)==(intf->gateway()&mask))
        {
            gtrace("<full scan>");
            g.mac_ = packet.ethHdr_->smac();//get mac
            g.ip_ = packet.arpHdr_->sip(); //get ip
            g.active = 1; //get active
            gtrace("%s",string(g.mac_).data());
            gtrace("%s",string(g.ip_).data());
        }

        fs.getMutex().lock();
        fs.getMap().insert(pair<WMac,Host>(g.mac_, g)); //insert into FSMap
        fs.getMutex().unlock();
        end = time(NULL);

        //time > 10seconds -> end
        if(end-start>10)return;
    }
}

