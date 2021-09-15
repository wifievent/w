#include "net/wscan.h"
int main()
{
    list<Scan::Guest> v;
    WNetInfo& wnetinfo = WNetInfo::instance();
    WRtm& rtm = wnetinfo.rtm();
    WRtmEntry* rtmentry = rtm.getBestEntry(WIp("8.8.8.8"));

    uint32_t ip_ = uint32_t(rtmentry->gateway());//gateway ip

    WPcapDevice device;
    device.open();//can find mymac, myip

    WPacket packet = WPacket();

    //dhcp
    thread dhcp(Scan::dhcp,&device,v);
    dhcp.join();
    //full-scan
    Scan::full_scan(&device,ip_,v);


    device.close();
    return 0;
}
