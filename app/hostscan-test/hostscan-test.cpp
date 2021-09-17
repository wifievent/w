#include "net/wscan.h"

int main()
{
    static mutex m;
    list<Host> v;
    WNetInfo& wnetinfo = WNetInfo::instance();
    WIntfList& intflist = wnetinfo.intfList();
    WRtm& rtm = wnetinfo.rtm();
    WRtmEntry* rtmentry = rtm.getBestEntry(WIp("8.8.8.8"));
    WIp gateway = rtmentry->gateway();//gateway ip

    WPcapDevice FSdevice,DHdevice,ARPdevice;
    FSdevice.open();//can find mymac, myip
    DHdevice.open();//can find mymac, myip
    ARPdevice.open();//can find mymac, myip

    WPacket packet = WPacket();

    //dhcp & full-scan
    Scan::open(&DHdevice,&FSdevice,gateway,&v,&m);

    NetBlock nb;
    //arp infection
    thread infect(NetBlock::infect,&ARPdevice,gateway,&v);

    //arp recover
    Host want;
    nb.recover(&ARPdevice, intflist, gateway, want);

    infect.join();

    FSdevice.close();
    DHdevice.close();
    ARPdevice.close();
    return 0;
}
