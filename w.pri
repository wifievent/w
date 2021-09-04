INCLUDEPATH *= $${PWD}/src
LIBS *= -L$${PWD}/bin -lw -lpcap

win32: DEFINES *= WOS_WIN
linux: DEFINES *= WOS_LINUX
macx: DEFINES *= WOS_MAC
