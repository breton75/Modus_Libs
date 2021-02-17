#ifndef CAN_DEFS_H
#define CAN_DEFS_H

#include <linux/can.h>

#include <QMap>

#include "../../../../../Modus/global/signal/sv_signal.h"
#include "../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../Modus/global/global_defs.h"
#include "../../../../../Modus/global/device/device_defs.h"

#define P_CANID   "canid"
#define P_OFFSET  "offset"
#define P_LEN     "len"

namespace can {

  struct DeviceParams {

//    quint8 param = 0;

    static DeviceParams fromJson(const QString& json_string) //throw (SvException)
    {
      QJsonParseError err;
      QJsonDocument jd = QJsonDocument::fromJson(json_string.toUtf8(), &err);

      if(err.error != QJsonParseError::NoError)
        throw SvException(err.errorString());

      try {

        return fromJsonObject(jd.object());

      }
      catch(SvException& e) {
        throw e;
      }
    }

    static DeviceParams fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      DeviceParams p;
      QString P;

      // canid
/*      P = P_CANID;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.src = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Адрес отправителя должен быть однобайтовым целым числом в шестнадцатиричном, восьмеричном или десятичном формате: [0xFF | 0377 | 255]"));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));
*/
      return p;

    }

    QString toString(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const
    {
      QJsonDocument jd;
      jd.setObject(toJsonObject());

      return QString(jd.toJson(format));
    }

    QJsonObject toJsonObject() const
    {
      QJsonObject j;

//      j.insert(P_SKM_SRC,               QJsonValue(QString("\"0x%1\"").arg(src, 2, 16, QChar('0'))).toString());

      return j;

    }
  };

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

}

#endif // CAN_DEFS_H

