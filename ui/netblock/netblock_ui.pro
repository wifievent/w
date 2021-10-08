QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DESTDIR = $${PWD}/../../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(../../w.pri)
SOURCES += \
    main.cpp \
    ../policy/policy.cpp \
    ../policy/policy_config.cpp \
    ../policy/policyobj.cpp \
    ../device-widget/devicewidget.cpp \
    ../device-widget/dinfo.cpp \
    ../../app/db-connect/db-connect.cpp \
    netblock_ui.cpp

HEADERS += \
    ../policy/policy.h \
    ../policy/policy_config.h \
    ../policy/policyobj.h \
    ../device-widget/devicewidget.h \
    ../device-widget/dinfo.h \
    ../../app/db-connect/db-connect.h \
    netblock_ui.h

FORMS += \
    ../policy/policy.ui \
    ../device-widget/devicewidget.ui \
    ../policy/policy_config.ui \
    netblock_ui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
