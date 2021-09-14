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
#include <vector>
#include <string>
#include <thread>
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

    static Etharp sendpacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
    static void scan(WPcapDevice* device, uint32_t ip_);
    static void acquire(WPcapDevice* device,vector<Guest> v,uint32_t ip_);
    static void dhcp(WPcapDevice* device,vector<Guest> v);

    static Scan& instance(){
        static Scan scan;
        return scan;
    }
};
#pragma pack(pop)
