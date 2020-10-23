#ifndef ISER_DEVICE_PARAMS
#define ISER_DEVICE_PARAMS

#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../global/ise_defs.h"
#include "../../../../svlib/sv_exception.h"


// имена параметров устройств
//#define P_ISER_ADDRESS        "address"
//#define P_ISER_SENDER_ADDRESS "sender_address"
//#define P_ISER_RESET_TIMEOUT  "reset_timeout"

//#define OPA_DEV_IMPERMISSIBLE_VALUE "Недопустимое значение параметра %1: %2.\n%3"
//#define OPA_DEV_NO_PARAM  "Для устройства не задан обязательный параметр \"%1\""

//#define ISER_DEFAULT_RESET_INTERVAL 10
//#define ISER_DEFAULT_ADDRESS        0
//#define ISER_DEFAULT_SENDER_ADDRESS 0


namespace iser {

  struct DeviceParams {

    quint16   iseid          = ISE_DEFAULT_ISEID;
    quint16   sender_iseid   = ISE_DEFAULT_SENDER_ISEID;
    quint16   reset_interval = ISE_DEFAULT_RESET_INTERVAL;

    static DeviceParams fromJson(const QString& json_string) throw (SvException)
    {
      QJsonParseError err;
      QJsonDocument jd = QJsonDocument::fromJson(json_string.toUtf8(), &err);

      if(err.error != QJsonParseError::NoError)
        throw SvException(err.errorString());

      try {

        return fromJsonObject(jd.object());

      }
      catch(SvException e) {
        throw e;
      }
    }

    static DeviceParams fromJsonObject(const QJsonObject &object) throw (SvException)
    {
      DeviceParams p;
      QString P;

      /* iseid */
      P = P_ISE_ISEID;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.iseid = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Идентификатор должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        p.iseid = ISE_DEFAULT_ISEID;

      /* sender_iseid */
      P = P_ISE_SENDER_ISEID;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.sender_iseid = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Иденитфикатор отправителя данных должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        p.sender_iseid = ISE_DEFAULT_SENDER_ISEID;


      /* reset_interval */
      P = P_ISE_RESET_TIMEOUT;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                                 .arg(P)
                                 .arg(object.value(P).toVariant().toString())
                                 .arg("Период сброса не может быть меньше 1 мсек."));

        p.reset_interval = object.value(P).toInt(ISER_DEFAULT_RESET_INTERVAL);

      }
      else
        p.reset_interval = ISE_DEFAULT_RESET_INTERVAL;


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
      QString r;

      r = QString::number(iseid, 16);
      QString iseid_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);

      r = QString::number(sender_iseid, 16);
      QString sender_iseid_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);

      j.insert(P_ISE_ISEID,         QJsonValue(iseid_r).toString("0x00"));
      j.insert(P_ISE_SENDER_ISEID,  QJsonValue(sender_iseid_r).toString("0x00"));
      j.insert(P_ISE_RESET_TIMEOUT, QJsonValue(reset_interval).toInt(ISE_DEFAULT_RESET_INTERVAL));

      return j;

    }
  };
}

#endif // ISER_DEVICE_PARAMS

