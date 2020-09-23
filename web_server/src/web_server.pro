#-------------------------------------------------
#
# Project created by QtCreator 2020-09-16T11:09:51
#
#-------------------------------------------------

QT       += network websockets
QT       -= gui

CONFIG += c++11 plugin

TARGET = /home/user/widen/lib/web_server
TEMPLATE = lib

DEFINES += WEBSERVER_LIBRARY

SOURCES += sv_web_server.cpp \
    ../../../WIDEN/global/sv_signal.cpp

HEADERS += sv_web_server.h\
        webserver_global.h \
    params.h \
    ../../../WIDEN/global/sv_abstract_server.h \
    ../../../WIDEN/global/sv_signal.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
