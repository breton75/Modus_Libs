QT -= gui

DEFINES += PROJ_12700_OHT_LIBRARY

CONFIG += c++11 plugin

TARGET = /home/user/Modus/lib/protocols/proj_12700_oht_agg
TEMPLATE = lib

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
    collection_0x13.cpp \
    collection_0x14.cpp \
    collection_0x19.cpp \
    collection_0x33.cpp \
    collection_status.cpp \
    proj_12700_oht.cpp \
    ../../../../../Modus/global/signal/sv_signal.cpp

HEADERS += \
    ../../../../../Modus/global/device/protocol/sv_abstract_protocol.h \
    ../../../../../Modus/global/global_defs.h \
    collection_0x13.h \
    collection_0x14.h \
    collection_0x19.h \
    collection_0x33.h \
    collection_status.h \
    oht_defs.h \
    oht_params.h \
    proj_12700_oht_global.h \
    proj_12700_oht.h \
    ../../../../../Modus/global/signal/sv_signal.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
