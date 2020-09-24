#-------------------------------------------------
#
# Project created by QtCreator 2020-09-24T18:46:45
#
#-------------------------------------------------

QT       += network

QT       -= gui

CONFIG += c++11 plugin

TARGET = proj_12700_opa
TEMPLATE = lib

DEFINES += PROJ_12700_OPA_LIBRARY

SOURCES += sv_opa.cpp \
    ../../../Modus/global/sv_signal.cpp \
    ../sv_opa_origin.cpp \
    ../sv_opa_thread.cpp

HEADERS += sv_opa.h \
    proj_12700_opa_global.h \
    ../../../Modus/global/sv_abstract_device.h \
    ../../../Modus/global/sv_signal.h \
    ../opa_defs.h \
    ../sv_opa_origin.h \
    ../sv_opa_thread.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
