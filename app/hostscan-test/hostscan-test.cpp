#include "net/wscan.h"

int main()
{
    list<Scan::Guest> v;
    WNetInfo& wnetinfo = WNetInfo::instance();
    WIntfList& intflist = wnetinfo.intfList();
    WRtm& rtm = wnetinfo.rtm();
    WRtmEntry* rtmentry = rtm.getBestEntry(WIp("8.8.8.8"));

    uint32_t ip_ = uint32_t(rtmentry->gateway());//gateway ip

    WPcapDevice device;
    device.open();//can find mymac, myip

    WPacket packet = WPacket();

    //dhcp
    thread dhcp(Scan::dhcp,&device,&v);

    //full-scan
    Scan::full_scan(&device,ip_,&v);

    //arp infection
    thread infect(Scan::send_arp,&device,ip_,&v);

    //arp recover
    WIp want;
    Scan::arp_recover(&device, want, ip_, intflist);

    dhcp.join();

    device.close();
    return 0;
}
