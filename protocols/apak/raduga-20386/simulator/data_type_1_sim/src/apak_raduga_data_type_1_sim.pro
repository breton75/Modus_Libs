QT -= gui

DEFINES += PROJ_APAK_RADUGA_DATATYPE1_LIBRARY

CONFIG += c++11 plugin

TARGET = /home/user/Modus/lib/protocols/apak_raduga_data_type_1_sim
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
    collection_0x02.cpp \
    collection_0x03.cpp \
    collection_0x19.cpp \
    collection_0x33.cpp \
    collection_status.cpp \
    ../../../../../Modus/global/signal/sv_signal.cpp \
    apak_raduga_data_type_1_sim.cpp \
    collection_data_type_1.cpp

HEADERS += \
    ../../../../../Modus/global/device/protocol/sv_abstract_protocol.h \
    ../../../../../Modus/global/global_defs.h \
    collection_0x02.h \
    collection_0x03.h \
    collection_0x19.h \
    collection_0x33.h \
    collection_status.h \
    apak_raduga_data_type_1_sim.h \
    apak_raduga_data_type_1_sim_global.h \
    ../../../../../Modus/global/signal/sv_signal.h \
    raduga_defs.h \
    protocol_params.h \
    collection_data_type_1.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
