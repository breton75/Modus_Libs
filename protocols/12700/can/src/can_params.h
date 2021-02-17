#ifndef CAN_PROTOCOL_PARAMS
#define CAN_PROTOCOL_PARAMS

#include <linux/can.h>

#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../../Modus/global/misc/sv_exception.h"
#include "../../../../../Modus/global/global_defs.h"
#include "../../../../../Modus/global/device/device_defs.h"

// имена параметров устройства
#define P_CAN_ID               "canid"
#define P_CAN_OFFSET           "offset"

namespace can {

  struct DeviceParams {

    canid_t canid  = 0;      // unsigned int 32
    quint8  offset = 0;

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
      P = P_CAN_ID;
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

      // offset
      P = P_CAN_OFFSET;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.dst = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Адрес получателя должен быть однобайтовым целым числом в шестнадцатиричном, восьмеричном или десятичном формате: [0xFF | 0377 | 255]"));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      // protocol version
      P = P_SKM_PROTOCOL_VERSION;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.protocol_version = h.toUShort(&ok, 0);

        if(!ok)
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                                 .arg(P)
                                 .arg(object.value(P).toVariant().toString())
                                 .arg("Версия протокола должна быть задана однобайтовым целым числом в шестнадцатиричном, восьмеричном или десятичном формате: [0xFF | 0377 | 255]"));
      }
      else
        p.protocol_version = DEFAULT_PROTOCOL_VERSION;


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

      j.insert(P_SKM_SRC,               QJsonValue(QString("\"0x%1\"").arg(src, 2, 16, QChar('0'))).toString());
      j.insert(P_SKM_DST,               QJsonValue(QString("\"0x%1\"").arg(dst, 2, 16, QChar('0'))).toString());
      j.insert(P_SKM_PROTOCOL_VERSION,  QJsonValue(QString("\"0x%1\"").arg(protocol_version, 2, 16, QChar('0'))).toString());

      return j;

    }
  };
}

#endif // CAN_PROTOCOL_PARAMS

