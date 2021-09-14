#include "net/wscan.h"
int main()
{
    Scan& scan = Scan::instance();

    vector<Scan::Guest> v;
    WNetInfo& wnetinfo = WNetInfo::instance();
    WRtm& rtm = wnetinfo.rtm();
    WRtmEntry* rtmentry = rtm.getBestEntry(WIp("8.8.8.8"));

    uint32_t ip_ = uint32_t(rtmentry->gateway());//gateway ip

    WPcapDevice device;
    device.open();//can find mymac, myip

    WPacket packet = WPacket();

    vector<thread> arp;

    //full-scan

    arp.push_back(std::thread(Scan::scan,&device,ip_));
    arp.push_back(std::thread(Scan::acquire,&device,v,ip_));

    //dhcp
    arp.push_back(std::thread(Scan::dhcp,&device,v));

    vector<thread>::iterator iter;
    for(iter = arp.begin();iter!=arp.end();iter++)
        (*iter).join();

    device.close();
    return 0;
}
