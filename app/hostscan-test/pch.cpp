#include "pch.h"

void sleepFunc(int msec) {
  #ifdef __linux__
  usleep(msec * 1000);
  #elif TARGET_OS_MAC
  unsleep(msec * 1000);
  #elif _WIN32
  sleep(msec);
  #endif
}