CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
include($${PWD}/../w.pri)
DESTDIR = $${PWD}/../bin

HEADERS += \
	../src/base/gtrace.h \
	../src/base/wbase.h \
	../src/base/wbuf.h \
	../src/base/wobj.h \
	../src/base/wstateobj.h \
	../src/capture/wcapture.h \
	../src/capture/wpcapcapture.h \
	../src/capture/wpcapdevice.h \
	../src/net/packet/wpacket.h \
	../src/net/pdu/warphdr.h \
	../src/net/pdu/wdhcphdr.h \
	../src/net/pdu/wethhdr.h \
	../src/net/pdu/wicmphdr.h \
	../src/net/pdu/wip6hdr.h \
	../src/net/pdu/wiphdr.h \
	../src/net/pdu/wpdu.h \
	../src/net/pdu/wtcphdr.h \
	../src/net/pdu/wudphdr.h \
	../src/net/wintf.h \
	../src/net/wintflist.h \
	../src/net/wip.h \
	../src/net/wip6.h \
	../src/net/wmac.h \
	../src/net/wnetinfo.h \
	../src/net/wrtm.h \
	../src/net/wrtmentry.h

SOURCES += \
	../src/base/gtrace.cpp \
	../src/base/wbase.cpp \
	../src/base/wbuf.cpp \
	../src/base/wobj.cpp \
	../src/base/wstateobj.cpp \
	../src/capture/wcapture.cpp \
	../src/capture/wpcapcapture.cpp \
	../src/capture/wpcapdevice.cpp \
	../src/net/packet/wpacket.cpp \
	../src/net/pdu/warphdr.cpp \
	../src/net/pdu/wdhcphdr.cpp \
	../src/net/pdu/wethhdr.cpp \
	../src/net/pdu/wicmphdr.cpp \
	../src/net/pdu/wip6hdr.cpp \
	../src/net/pdu/wiphdr.cpp \
	../src/net/pdu/wpdu.cpp \
	../src/net/pdu/wtcphdr.cpp \
	../src/net/pdu/wudphdr.cpp \
	../src/net/wintf.cpp \
	../src/net/wintflist.cpp \
	../src/net/wip.cpp \
	../src/net/wip6.cpp \
	../src/net/wmac.cpp \
	../src/net/wnetinfo.cpp \
	../src/net/wrtm.cpp \
	../src/net/wrtmentry.cpp
