QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
LIBS += -lcurl
CONFIG += c++11
include(../../w.pri)
DESTDIR = $${PWD}/../../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    device/devicewidget.cpp \
    device/dinfo.cpp \
    policy/policy.cpp \
    policy/policy_config.cpp \
    policy/policyobj.cpp \
    policy/policytimeedit.cpp \
    netblock_ui.cpp \
    core/arppacket.cpp \
    core/core.cpp \
    core/fullscan.cpp \
    core/netblock.cpp \
    core/oui.cpp \
    core/packet.cpp \
    core/parser.cpp \
    main.cpp


HEADERS += \
    device/devicewidget.h \
    device/dinfo.h \
    policy/policy.h \
    policy/policy_config.h \
    policy/policyobj.h \
    policy/policytimeedit.h \
    netblock_ui.h \
    core/arppacket.h \
    core/core.h \
    core/fullscan.h \
    core/netblock.h \
    core/oui.h \
    core/packet.h \
    core/parser.h \
    core/pch.h \
    db_check.h

FORMS += \
    device/devicewidget.ui \
    policy/policy.ui \
    policy/policy_config.ui \
    netblock_ui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
