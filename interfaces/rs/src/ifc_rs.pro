QT -= gui
QT += serialport

TEMPLATE = lib
DEFINES += IFC_RS_LIBRARY

CONFIG += c++11 plugin

VERSION = 1.0.1    # major.minor.patch
DEFINES +=  LIB_VERSION=\\\"$$VERSION\\\"
DEFINES += "LIB_AUTHOR=\"\\\"Свиридов С. А.\\\"\""

TARGET = /home/user/Modus/lib/interfaces/rs
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sv_rs.cpp

HEADERS += \
    ../../../../Modus/global/device/interface/sv_abstract_interface.h \
    ../../../../Modus/global/device/device_defs.h \
    ifc_rs_global.h \
    sv_rs.h \
    rs_defs.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
