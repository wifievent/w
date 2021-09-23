#include "dhcp.h"
int main()
{
    Scan sc;
    SendArp sa;
    sc.DHdevice.open();
    sc.FSdevice.open();//can find mymac, myip
    sa.ARPdevice.open();//can find mymac, myip

    WPacket packet = WPacket();

    sc.open(&sc);
    sc.findName();
    sa.v = sc.v;

    //arp infection
    //thread infect(&SendArp::infect,&sa);

    //arp recover
    /*Host want;
    sa.recover(want);
*/
    //infect.join();

    sc.FSdevice.close();
    sc.DHdevice.close();
    sa.ARPdevice.close();
    return 0;
}
