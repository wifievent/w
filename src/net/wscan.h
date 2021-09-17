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
#include <mutex>
using namespace std;

#pragma pack(push,1)
struct Etharp{
    struct WEthHdr eth;
    struct WArpHdr arp;
    static Etharp makeArppacket(WMac dmac, WMac smac, WMac tmac,WIp tip, WIp sip);
};

struct Host {
  char* name;
  WMac mac_;
  WIp ip_;
};

struct NetBlock {
  void open();
  void close();
  static void infect(WPcapDevice* device, WIp gateway,list<Host>* v);
  void recover(WPcapDevice* device, WIntfList& intflist, WIp gateway, Host host);
};

struct Scan {
  static void open(WPcapDevice* DHdevice, WPcapDevice* FSdevice, WIp gateway, list<Host>* v, mutex* m); // start fullscan and dhcpScan thread
  void close(); // stop fullscan and dhcpScan thread
  static void acquire(WPcapDevice* device, list<Host>* v, WIp gateway, mutex* m);
  static void scan(WPcapDevice* device, WIp gateway);
  static void dhcpScan(WPcapDevice* device, list<Host>* v, mutex* m);
  virtual void onNewHost(Host host); // event when new host is detected
};
#pragma pack(pop)
