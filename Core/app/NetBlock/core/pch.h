#pragma once
#include <QtGlobal>
#include "net/wrtm.h"
#include "net/packet/wpacket.h"
#include "net/wintflist.h"
#include "base/wobj.h"
#include <typeinfo>
#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include <thread>
#include <time.h>
#include <mutex>
#include <sys/time.h>
#include <QString>
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
  int connect_time = 600;

  bool isConnected() {
      struct timeval now;
      gettimeofday(&now, NULL);
      GTRACE("isConnected = %s",std::string(ip_).data());
      GTRACE("now = %d",now.tv_sec);
      GTRACE("Last = %d\n",last.tv_sec);
      return now.tv_sec - last.tv_sec < connect_time;
  }
};
