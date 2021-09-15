#include "pdu/wethhdr.h"
#include "pdu/warphdr.h"
#include "capture/wpcapdevice.h"
#include "wrtm.h"
#include "wip.h"
#include "wmac.h"
#include "wintflist.h"
#include "packet/wpacket.h"
#include "pdu/wdhcphdr.h"
#include "typeinfo"
#include "iostream"
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#include <thread>
#include <time.h>
using namespace std;

#pragma pack(push,1)
struct Scan {
private:
    Scan();
    ~Scan();

public:
    struct Etharp{
        struct WEthHdr eth;
        struct WArpHdr arp;
    };

    struct Guest{
        WMac mac;
        WIp ip;
        char* name;
    };

    static Etharp makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
    static void scan(WPcapDevice* device, uint32_t ip_);
    static void acquire(WPcapDevice* device,list<Guest>* v,uint32_t ip_);
    static void dhcp(WPcapDevice* device,list<Guest>* v);
    static void full_scan(WPcapDevice* device, uint32_t ip_,list<Guest>* v);
    static void send_arp(WPcapDevice* device,uint32_t ip_,list<Guest>* v);
    static Scan& instance(){
        static Scan scan;
        return scan;
    }
};
#pragma pack(pop)
