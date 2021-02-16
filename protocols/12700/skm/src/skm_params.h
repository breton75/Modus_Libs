#ifndef SKM_DEVICE_PARAMS
#define SKM_DEVICE_PARAMS

#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../../Modus/global/misc/sv_exception.h"
#include "../../../../../Modus/global/global_defs.h"
#include "../../../../../Modus/global/device/device_defs.h"

// имена параметров устройства
#define P_SKM_SRC               "src"
#define P_SKM_DST               "dst"
#define P_SKM_PROTOCOL_VERSION  "protocol_version"

#define DEFAULT_SRC               0x01
#define DEFAULT_DST               0x09
#define DEFAULT_PROTOCOL_VERSION  0x24

namespace skm {

  struct DeviceParams {

    quint8 src = 0;
    quint8 dst = 0;
    quint8 protocol_version = 0x24;

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

      // src
      P = P_SKM_SRC;
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
        p.src = DEFAULT_SRC;

      // dst
      P = P_SKM_DST;
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
        p.dst = DEFAULT_DST;

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

#endif // DEVICEPARAMS

