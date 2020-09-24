#-------------------------------------------------
#
# Project created by QtCreator 2020-09-16T11:09:51
#
#-------------------------------------------------

QT       += network
QT       -= gui

CONFIG += c++11 plugin

TARGET = /home/user/Modus/lib/web_server
TEMPLATE = lib

DEFINES += WEBSERVER_LIBRARY

SOURCES += sv_web_server.cpp \
    ../../../Modus/global/sv_signal.cpp

HEADERS += sv_web_server.h\
        webserver_global.h \
    params.h \
    ../../../Modus/global/sv_abstract_server.h \
    ../../../Modus/global/sv_signal.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
