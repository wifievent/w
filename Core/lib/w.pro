CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
CONFIG += c99
CONFIG += W_BUILD
include($${PWD}/../w.pri)
DESTDIR = $${PWD}/../bin

HEADERS += \
	../src/base/gtrace.h \
	../src/base/json/json-forwards.h \
	../src/base/json/json.h \
	../src/base/wbase.h \
	../src/base/wbuf.h \
	../src/base/wjson.h \
	../src/base/wobj.h \
	../src/base/wstateobj.h \
	../src/base/db-connect.h \
	../src/net/capture/wcapture.h \
	../src/net/capture/wpcapcapture.h \
	../src/net/capture/wpcapdevice.h \
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
	../src/net/process/wpacketdbg.h \
	../src/net/wintf.h \
	../src/net/wintflist.h \
	../src/net/wip.h \
	../src/net/wip6.h \
	../src/net/wmac.h \
	../src/net/wnet.h \
	../src/net/wnetinfo.h \
	../src/net/wreadable.h \
	../src/net/wrtm.h \
	../src/net/wrtmentry.h \
	../src/net/wwritable.h \
	../src/base/sqlite/sqlite3.h

SOURCES += \
	../src/base/gtrace.cpp \
	../src/base/jsoncpp.cpp \
	../src/base/wbase.cpp \
	../src/base/wbuf.cpp \
	../src/base/wjson.cpp \
	../src/base/wobj.cpp \
	../src/base/wstateobj.cpp \
	../src/base/db-connect.cpp \
	../src/net/capture/wcapture.cpp \
	../src/net/capture/wpcapcapture.cpp \
	../src/net/capture/wpcapdevice.cpp \
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
	../src/net/process/wpacketdbg.cpp \
	../src/net/wintf.cpp \
	../src/net/wintflist.cpp \
	../src/net/wip.cpp \
	../src/net/wip6.cpp \
	../src/net/wmac.cpp \
	../src/net/wnet.cpp \
	../src/net/wnetinfo.cpp \
	../src/net/wreadable.cpp \
	../src/net/wrtm.cpp \
	../src/net/wrtmentry.cpp \
	../src/net/wwritable.cpp \
	../src/base/sqlite/sqlite3.c

win32 {
HEADERS += \
	../src/net/_win/wipadapterinfo.h \
	../src/net/_win/wipforwardtable.h

SOURCES += \
	../src/net/_win/wipadapterinfo.cpp \
	../src/net/_win/wipforwardtable.cpp
}
