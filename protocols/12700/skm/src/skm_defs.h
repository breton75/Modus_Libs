#ifndef SKM_DEFS_H
#define SKM_DEFS_H

#include <QMap>

#include "../../../Modus/global/signal/sv_signal.h"
#include "../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../Modus/global/global_defs.h"
#include "../../../Modus/global/device/device_defs.h"

#define TYPE_0x01 0x01
#define TYPE_0x02 0x02

#define P_SKM_VIN     "vin"
#define P_SKM_SENSOR  "sensor"
#define P_SKM_FAKTOR  "faktor"
#define P_SKM_ROOM    "room"
#define P_SKM_LEVEL   "level"
#define P_SKM_BYTE    "byte"
#define P_SKM_BIT     "bit"


namespace skm {

  struct DATA
  {
    DATA() {}

    QByteArray data = QByteArray();
//    qint8   data[MAX_BUF_SIZE];
    quint8  data_type;
    quint8  data_length;
    quint16 crc;

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

    virtual void updateSignals(const skm::DATA* data = nullptr) = 0;

  protected:

    inline bool check_1F_2F_55(quint8 byte)
    {
      return ((byte == 0x1F) || (byte == 0x2F) || (byte == 0x55));
    }

    inline quint32 getUid(quint8 val1, quint8 val2, quint8 val3, quint8 val4)
    {
      return (static_cast<quint32>(val1) << 24) + (static_cast<quint32>(val2) << 16) + (static_cast<quint32>(val3) << 8) + static_cast<quint32>(val4);
    }
  };
}

#endif // SKM_DEFS_H




