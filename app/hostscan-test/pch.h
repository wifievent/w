#pragma once
#include "net/capture/wpcapdevice.h"
#include "net/wrtm.h"
#include "net/wintflist.h"
#include "net/packet/wpacket.h"

#include <typeinfo>
#include <iostream>
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
static bool check = true;
