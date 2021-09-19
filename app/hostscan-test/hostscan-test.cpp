#include "dhcp.h"
int main()
{
    Scan sc;
    SendArp sa;
    sc.DHdevice.open();
    sc.FSdevice.open();//can find mymac, myip
    sa.ARPdevice.open();//can find mymac, myip

    WPacket packet = WPacket();

    //Scan sc;
    //sc.open(&dhcp); --> error generated

    //dhcp & full-scan -> contents in open function
    thread dhcp = thread(&Scan::dhcpScan,&sc);
    thread scan_(&Scan::scan,&sc);
    thread acquire_(&Scan::acquire,&sc);
    dhcp.detach();
    scan_.join();
    acquire_.join();

    //arp infection
    thread infect(&SendArp::infect,&sa);

    //arp recover
    Host want;
    sa.recover(want);

    infect.join();

    sc.FSdevice.close();
    sc.DHdevice.close();
    sa.ARPdevice.close();
    return 0;
}
