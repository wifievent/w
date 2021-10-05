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
    parser.cpp
    /home/bob/project/host-list/w/app/db-connect/db-connect.cpp \
    /home/bob/project/host-list/w/app/db-connect/sqlite3.c

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
