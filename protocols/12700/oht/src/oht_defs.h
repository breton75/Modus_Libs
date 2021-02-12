#ifndef OHT_DEFS_H
#define OHT_DEFS_H

#include <QMap>

#include "../../../../../Modus/global/signal/sv_signal.h"
#include "../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../Modus/global/global_defs.h"
#include "../../../../../Modus/global/device/device_defs.h"

#define TYPE_0x33 0x33
#define TYPE_0x13 0x13
#define TYPE_0x14 0x14
#define TYPE_0x19 0x19

#define P_ROUTE   "route"
#define P_LEN     "len"
#define P_BYTE    "byte"
#define P_BIT     "bit"
#define P_NUMBER  "number"


namespace oht {

  struct DATA
  {
    DATA() {}

//    qint8   data[MAX_BUF_SIZE];
//    quint8  data_length;

    QByteArray data = QByteArray();
    quint8     data_type;
    quint16    crc;

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

    virtual void updateSignals(const oht::DATA* data = nullptr) = 0;

  protected:

    inline quint32 getUid(quint8 val1, quint8 val2, quint8 val3, quint8 val4)
    {
      return (static_cast<quint32>(val1) << 24) + (static_cast<quint32>(val2) << 16) + (static_cast<quint32>(val3) << 8) + static_cast<quint32>(val4);
    }
  };
}

#endif // OHT_DEFS_H




