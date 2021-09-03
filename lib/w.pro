CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

DESTDIR = $${PWD}/../bin

HEADERS += \
	../src/gtrace.h \
	../src/wcore.h \
	../src/wintf.h \
	../src/wip.h \
	../src/wmac.h \
	../src/wintflist.h

SOURCES += \
	../src/gtrace.cpp \
	../src/wcore.cpp \
	../src/wintf.cpp \
	../src/wip.cpp \
	../src/wmac.cpp \
	../src/wintflist.cpp

