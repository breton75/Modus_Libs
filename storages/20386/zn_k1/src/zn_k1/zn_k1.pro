#-------------------------------------------------
#
# Project created by QtCreator 2021-09-03T14:12:02
#
#-------------------------------------------------

QT       += network dbus

QT       -= gui

CONFIG += c++11 plugin

TARGET = /home/user/Modus/lib/zn_writer
TEMPLATE = lib

DEFINES += ZN_K1_LIBRARY

SOURCES += \
    zn_writer.cpp

HEADERS +=\
    zn_writer.h \
    zn_writer_defs.h \
    zn_writer_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
