TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
include(../../w.pri)
DESTDIR = $${PWD}/../../bin
SOURCES += \
    arppacket.cpp \
    core.cpp \
    fullscan.cpp \
    hostscan-test.cpp \
    netblock.cpp \
    packet.cpp \
<<<<<<< HEAD
    parser.cpp
=======
    parser.cpp \
    pch.cpp
>>>>>>> feature/code-integrated

HEADERS += \
    arppacket.h \
    core.h \
    fullscan.h \
    netblock.h \
    packet.h \
    parser.h \
    pch.h

DISTFILES += \
    test.db
