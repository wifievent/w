TEMPLATE = app
CONFIG += c++11 console
CONFIG -= app_bundle
CONFIG -= qt
include(../../w.pri)
DESTDIR = $${PWD}/../../bin

SOURCES += \
    proxy-test.cpp

HEADERS += \
	stdafx.h