#-------------------------------------------------
#
# Project created by QtCreator 2020-09-24T18:46:45
#
#-------------------------------------------------

QT       += network serialport

QT       -= gui

CONFIG += c++11 plugin

TARGET = /home/user/Modus/lib/proj_12700_opa_aggregate
TEMPLATE = lib

DEFINES += PROJ_12700_OPA_LIBRARY

SOURCES += sv_opa.cpp \
    ../../../Modus/global/sv_signal.cpp \
    collection_0x02.cpp \
    collection_status.cpp \
    collection_0x03.cpp \
    collection_0x04.cpp \
    collection_0x19.cpp \
    collection_0x33.cpp

HEADERS += sv_opa.h \
    proj_12700_opa_global.h \
    ../../../Modus/global/sv_abstract_device.h \
    ../../../Modus/global/sv_signal.h \
    opa_defs.h \
    device_params.h \
    ifc_udp_params.h \
    ifc_serial_params.h \
    collection_status.h \
    collection_0x02.h \
    collection_0x03.h \
    collection_0x04.h \
    collection_0x19.h \
    collection_0x33.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
