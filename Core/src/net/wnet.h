#pragma once

#include "base/wbase.h"

typedef uint8_t le8_t;
typedef uint16_t le16_t;
typedef uint32_t le32_t;
typedef uint64_t le64_t;

#ifdef WOS_LINUX
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif // WOS_LINUX

#ifdef WOS_WIN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#endif // WOS_WIN
