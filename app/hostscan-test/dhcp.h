#include "net/pdu/wethhdr.h"
#include "net/pdu/warphdr.h"
#include "net/capture/wpcapdevice.h"
#include "net/wrtm.h"
#include "net/wip.h"
#include "net/wmac.h"
#include "net/wintflist.h"
#include "net/packet/wpacket.h"
#include "net/pdu/wdhcphdr.h"
#include "typeinfo"
#include "iostream"
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <string>
#include <thread>
#include <time.h>
#include <mutex>
using namespace std;

#pragma pack(push,1)
struct Etharp{
    struct WEthHdr eth;
    struct WArpHdr arp;
    void makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
};

struct Host {
  char* name;
  WMac mac_;
  WIp ip_;
};

struct SendArp {
  WPcapDevice ARPdevice;
  list<Host> v;//host to manage
  WMac mac_gate;
  void open();
  void close();//if program end -> full recover arp
  void infect();
  void recover(Host want);
};

struct Scan {
  WPcapDevice DHdevice, FSdevice;
  list<Host> v;//full host
  mutex m;
  void open(thread* dhcp); // start fullscan and dhcpScan thread
  void close(); // stop fullscan and dhcpScan thread
  void acquire();
  void scan();
  void dhcpScan();
  //virtual void onNewHost(Host host); // event when new host is detected
};
#pragma pack(pop)
