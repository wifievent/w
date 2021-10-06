QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
include(../../w.pri)
DESTDIR = $${PWD}/../../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    netblock.cpp \
    policy.cpp \
    policy_config.cpp \
    policyobj.cpp \
    devicewidget.cpp \
    dinfo.cpp \
    dthread.cpp \
    arppacket.cpp \
    core.cpp \
    fullscan.cpp \
    hostscan-test.cpp \
    netblock.cpp \
    packet.cpp \
    parser.cpp \
    pch.cpp

HEADERS += \
    netblock.h \
    policy.h \
    policy_config.h \
    policyobj.h \
    devicewidget.h \
    dinfo.h \
    dthread.h \
    arppacket.h \
    core.h \
    fullscan.h \
    netblock.h \
    packet.h \
    parser.h \
    pch.h \
    db_check.h

FORMS += \
    netblock.ui \
    policy.ui \
    devicewidget.ui \
    policy_config.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
