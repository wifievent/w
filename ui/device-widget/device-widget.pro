QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DESTDIR = $${PWD}/../../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../../w.pri)

SOURCES += \
    devicewidget.cpp \
    dinfo.cpp \
    main.cpp \
    ../../app/db-connect/db-connect.cpp
    ../../app/hostscan-test/fullscan.cpp

HEADERS += \
    devicewidget.h \
    dinfo.h \
    ../../app/db-connect/db-connect.h
    ../../app/hostscan-test/fullscan.h

FORMS += \
    devicewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

linux: LIBS *= -lsqlite3 -lpthread -ldl
