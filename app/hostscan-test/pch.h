#pragma once
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
#include <map>
#include <string>
#include <thread>
#include <time.h>
#include <mutex>
using namespace std;

struct EthArp{
    struct WEthHdr eth;
    struct WArpHdr arp;
};

struct Host {
  char* name;
  WMac mac_;
  WIp ip_;
  int active;
};
