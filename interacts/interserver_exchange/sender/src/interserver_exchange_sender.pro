#-------------------------------------------------
#
# Project created by QtCreator 2020-08-31T13:48:34
#
#-------------------------------------------------

QT       += network

QT       -= gui

CONFIG += c++11 plugin

TARGET = /home/user/Modus/lib/interserver_exchange_sender
TEMPLATE = lib

DEFINES += INTERSERVER_EXCHANGE_SENDER_LIBRARY

SOURCES += \
    ../../../../Modus/global/sv_signal.cpp \
    sv_ises.cpp

HEADERS += \
    interserver_exchange_sender_global.h \
    ../../../../Modus/global/sv_abstract_storage.h \
    ../../../../svlib/sv_exception.h \
    ../../../../Modus/global/sv_signal.h \
    ../../global/ise_defs.h \
    sv_ises.h \
    storage_params.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
