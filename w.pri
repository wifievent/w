INCLUDEPATH *= $${PWD}/src
LIBS *= -L$${PWD}/bin -lw

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
	INCLUDEPATH *= $${PWD}/../npcap/include
	LIBS *= -L$${PWD}/../npcap/Lib/x64
	LIBS *= -lwpcap -lpacket -lws2_32 -liphlpapi
}

linux: LIBS *= -lpcap
