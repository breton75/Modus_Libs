﻿#ifndef OPA_DEFS_H
#define OPA_DEFS_H

#include <QMap>

#include "../../../../../Modus/global/signal/sv_signal.h"
#include "../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../Modus/global/global_defs.h"
#include "../../../../../Modus/global/device/device_defs.h"

#define TYPE_0x33 0x33
#define TYPE_0x02 0x02
#define TYPE_0x03 0x03
#define TYPE_0x04 0x04
#define TYPE_0x19 0x19

#define P_OPA_SENSOR  "sensor"
#define P_OPA_FAKTOR  "faktor"
#define P_OPA_ROOM    "room"
#define P_OPA_LEVEL   "level"
#define P_OPA_BYTE    "byte"
#define P_OPA_BIT     "bit"


namespace opa {

  struct DATA
  {
    DATA():
      data(nullptr),
      bufsize(0)
    {  }

    DATA(quint16 size):
      data(nullptr),
      bufsize(size)
    {
      data = (quint8*)malloc(size);
    }

    ~DATA()
    {
      if(data)
        free(data);
    }

    bool resize(quint16 size)
    {
      if(data)
        free(data);

      data = nullptr;

      bufsize = size;
      data = (quint8*)malloc(size);

      return bool(data);
    }

    quint8* data = nullptr;
    quint8  type;
    quint8  len;
    quint16 crc;

    quint16 bufsize;

  };

  class SvAbstractSignalCollection: public QObject
  {
    Q_OBJECT

  public:
    SvAbstractSignalCollection()
    {  }

    virtual ~SvAbstractSignalCollection()
    {  }

    virtual void addSignal(modus::SvSignal* signal) = 0;

    virtual void updateSignals(const opa::DATA* data = nullptr) = 0;

  };
}

#endif // OPA_DEFS_H




