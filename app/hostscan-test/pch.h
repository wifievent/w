#pragma once
#include "net/wrtm.h"
#include "net/packet/wpacket.h"
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
  string name;
  WMac mac_;
  WIp ip_;
  struct timeval last;
  bool isConnected(){
      struct timeval now;
      gettimeofday(&now,NULL);
      return now.tv_sec - last.tv_sec < 60;
  }
};
