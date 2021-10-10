#pragma once
#include "net/wrtm.h"
#include "net/packet/wpacket.h"
#include "net/wintflist.h"

#include <x86_64-linux-gnu/curl/curl.h>
#include <typeinfo>
#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include <thread>
#include <time.h>
#include <mutex>
#include <sys/time.h>

#include <stdlib.h>
#include <unistd.h>

struct EthArp{
    WEthHdr eth;
    WArpHdr arp;
};

struct Host {
  std::string name;
  WMac mac_;
  WIp ip_;
  struct timeval last;

  bool isConnected() {
      struct timeval now;
      gettimeofday(&now, NULL);
      gtrace("%d",now.tv_sec);
      gtrace("%d",last.tv_sec);
      return now.tv_sec - last.tv_sec < 60;
  }
};
