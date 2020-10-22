#-------------------------------------------------
#
# Project created by QtCreator 2020-09-24T18:46:45
#
#-------------------------------------------------

QT       += network serialport

QT       -= gui

CONFIG += c++11 plugin

TARGET = /home/user/Modus/lib/interserver_exchange_receiver
TEMPLATE = lib

DEFINES += INTERSERVER_EXCHANGE_RECEIVER_LIBRARY

SOURCES += sv_iser.cpp \
    ../../../../Modus/global/sv_signal.cpp

HEADERS += sv_iser.h \
    interserver_exchange_receiver_global.h \
    ../../../../Modus/global/sv_abstract_device.h \
    ../../../../Modus/global/sv_signal.h \
    ../../global/ise_defs.h \
    device_params.h \
    ifc_udp_params.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
