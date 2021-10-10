#pragma once

#include "pch.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};

std::string UrlEncode(const std::string &s);
QString oui_db(WMac mac);
