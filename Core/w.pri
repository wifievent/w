#
# include path
#
INCLUDEPATH *= $${PWD}/src

#
# link library
#
LIBS *= -L$${PWD}/bin -lw -pthread

!CONFIG(W_BUILD) {
   PRE_TARGETDEPS *= $${PWD}/bin/libw.a
}

#
# define
#
win32: DEFINES *= WOS_WIN
linux: DEFINES *= WOS_LINUX
macx: DEFINES *= WOS_MAC

#
# pcap
#
win32 {
        INCLUDEPATH *= $${PWD}/../../npcap/include
        LIBS *= -L$${PWD}/../../npcap/Lib/x64
        LIBS *= -lwpcap -lpacket -lws2_32 -liphlpapi
}

#
# debug and release
#
CONFIG(debug, debug|release) DEFINES *= _DEBUG
CONFIG(release, debug|release) DEFINES *= _RELEASE
CONFIG *= c++11 force_debug_info

linux: LIBS *= -lpthread -ldl -lpcap

win32: LIBS += -L$$PWD/../../curl/lib/ -lcurl

INCLUDEPATH += $$PWD/../../curl/include
DEPENDPATH += $$PWD/../../curl/include

