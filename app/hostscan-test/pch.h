#pragma once
#include "net/packet/wpacket.h"
#include "net/wrtm.h"
#include "net/wintflist.h"

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
    WEthHdr eth;
    WArpHdr arp;
};

struct Host {
  char* name;
  WMac mac_;
  WIp ip_;
  bool is_connect;
};
static bool check = true;
