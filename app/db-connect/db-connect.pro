QT += core
QT += sql
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
include(../../w.pri)
DESTDIR = $${PWD}/../../bin
HEADERS += \
	db-connect.h \
        sqlite3.h
SOURCES += \
	db-connect.cpp \
        sqlite3.c
#
# DB-connect
#

linux: LIBS *= -lsqlite3 -lpthread -ldl
