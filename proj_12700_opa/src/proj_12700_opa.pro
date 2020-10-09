#-------------------------------------------------
#
# Project created by QtCreator 2020-09-24T18:46:45
#
#-------------------------------------------------

QT       += network serialport

QT       -= gui

CONFIG += c++11 plugin

TARGET = proj_12700_opa
TEMPLATE = lib

DEFINES += PROJ_12700_OPA_LIBRARY

SOURCES += sv_opa.cpp \
    ../../../Modus/global/sv_signal.cpp

HEADERS += sv_opa.h \
    proj_12700_opa_global.h \
    ../../../Modus/global/sv_abstract_device.h \
    ../../../Modus/global/sv_signal.h \
    opa_defs.h \
    device_params.h \
    ifc_udp_params.h \
    ifc_serial_params.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
