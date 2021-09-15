#include "wscan.h"

Scan::Etharp Scan::makearppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip){
    Etharp etharp;
    etharp.eth.dmac_ = dmac;
    etharp.eth.smac_ = smac;
    etharp.eth.type_ = htons(WEthHdr::Arp);

    etharp.arp.hrd_ = htons(WArpHdr::ETHER);
    etharp.arp.pro_ = htons(WEthHdr::Ip4);
    etharp.arp.hln_ = WMac::SIZE;
    etharp.arp.pln_ = WIp::SIZE;
    etharp.arp.op_ = htons(WArpHdr::Request);
    etharp.arp.smac_ = smac;
    etharp.arp.sip_ = htonl(sip);
    etharp.arp.tmac_ = tmac;
    etharp.arp.tip_ = htonl(tip);
    return etharp;
}

void Scan::scan(WPcapDevice* device, uint32_t ip_){
    char* gateway = (char*)malloc(sizeof(char)*4);
    WPacket packet = WPacket();
    packet.buf_.size_ = sizeof(Etharp);
    Etharp etharp = makearppacket(WMac("FF:FF:FF:FF:FF:FF"),device->intf()->mac(),WMac("00:00:00:00:00:00"),WIp("0.0.0.0"),device->intf()->ip());

    for(int l = 1; l<255; l++){
        if((ip_&0x000000FF)==(l&0x000000FF))continue;
        sprintf(gateway, "%u.%u.%u.%u",
            (ip_ & 0xFF000000) >> 24,
            (ip_ & 0x00FF0000) >> 16,
            (ip_ & 0x0000FF00) >> 8,
            (l & 0x000000FF));
        string str(gateway);
        etharp.arp.tip_ = htonl(WIp(str));
        packet.buf_.data_ = reinterpret_cast<byte*>(&etharp);
        device->write(packet.buf_);
    }
    if(gateway){
        free(gateway);
        gateway = NULL;
    }
}

void Scan::full_scan(WPcapDevice* device, uint32_t ip_, list<Scan::Guest> v){
    thread thread1(Scan::scan,device,ip_);
    thread thread2(Scan::acquire,device,v,ip_);
    thread2.join();
}

void Scan::acquire(WPcapDevice* device,list<Guest> v,uint32_t ip_)
{
    WPacket packet = WPacket();
    time_t start, end;
    start = time(NULL);
    while(1)
    {
        if(device->WPcapCapture::read(&packet)==WPacket::Result::Ok)
        {
            if(packet.ethHdr_->type()!=WEthHdr::Arp)continue;
            if(packet.ethHdr_->dmac_!=device->intf()->mac()||packet.arpHdr_->op()!=packet.arpHdr_->Reply)continue;

            int flag1 = 0, flag2 = 0, flag3 = 0;
            (packet.arpHdr_->sip() & 0xFF000000) >> 24 == (ip_& 0xFF000000)>>24 ? flag1 = 1: flag1 = 0;
            (packet.arpHdr_->sip() & 0x00FF0000) >> 16 == (ip_& 0x00FF0000) >> 16 ? flag2 = 1: flag2 = 0;
            (packet.arpHdr_->sip() & 0x0000FF00) >> 8 == (ip_& 0x0000FF00) >> 8 ? flag3 = 1: flag3 = 0;

            if(flag1&&flag2&&flag3)
            {
                Guest g;
                g.mac = packet.ethHdr_->smac();
                g.ip = packet.arpHdr_->sip();
                v.push_back(g);
            }
        }
        end = time(NULL);
        printf("%f",(double)(end-start));
        if(end-start>10)return;
    }
}

void Scan::dhcp(WPcapDevice* device,list<Guest> v)
{
    WPacket packet = WPacket();
    while(1)
    {
        if(device->WPcapCapture::read(&packet)==WPacket::Result::Ok)
        {
            if(packet.ethHdr_->type()!=WEthHdr::Ip4)continue;
            if(packet.ipHdr_->p()!=WIpHdr::Udp)continue;
            if(packet.udpHdr_->sport()!=67&&packet.udpHdr_->dport()!=67)continue;//dhcp

            packet.dhcpHdr_ = (WDhcpHdr*)packet.udpData_.data_;

            Guest g;
            g.mac = packet.dhcpHdr_->clientMac_;//get mac

            for(WDhcpHdr::Option* opt = packet.dhcpHdr_->first(); opt!=nullptr; opt=opt->next())
            {
                if(opt->type_ == 50)//get ip
                {
                    char* buf = (char*)malloc(sizeof(char)*4);
                    for(int i = 0; i<opt->len_-1; i++)
                        sprintf(buf+4*i,"%03d.",*(&opt->len_+1+i));
                    sprintf(buf+12,"%03d",*(&opt->len_+4));

                    string str(buf);
                    g.ip = WIp(str);
                }
                else if(opt->type_ == 12)//get name
                {
                    g.name = (char*)malloc(sizeof(char)*opt->len_);
                    for(int i = 0; i<opt->len_;i++)
                        g.name[i] = *(&opt->len_+1+i);
                }
            }
            v.push_back(g);
        }
    }
}
