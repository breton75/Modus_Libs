#-------------------------------------------------
#
# Project created by QtCreator 2020-09-16T11:09:51
#
#-------------------------------------------------

QT       += network
QT       -= gui

CONFIG += c++11 plugin

TARGET = /home/developer/Modus/lib/restapi_server
TEMPLATE = lib

DEFINES += INTERACT_LIBRARY

SOURCES += sv_restapi_server.cpp \
    ../../../Modus/global/signal/sv_signal.cpp

HEADERS += sv_restapi_server.h \
        restapi_server_global.h \
    params.h \
    ../../../Modus/global/interact/sv_abstract_interact.h \
    ../../../Modus/global/signal/sv_signal.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
