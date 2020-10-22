#ifndef ISER_DEVICE_PARAMS
#define ISER_DEVICE_PARAMS

#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../global/ise_defs.h"
#include "../../../../svlib/sv_exception.h"


// имена параметров устройств
#define P_ISER_ADDRESS        "address"
#define P_ISER_SENDER_ADDRESS "sender_address"
#define P_ISER_RESET_TIMEOUT  "reset_timeout"

//#define OPA_DEV_IMPERMISSIBLE_VALUE "Недопустимое значение параметра %1: %2.\n%3"
//#define OPA_DEV_NO_PARAM  "Для устройства не задан обязательный параметр \"%1\""

#define ISER_DEFAULT_RESET_INTERVAL 10
#define ISER_DEFAULT_ADDRESS        0
#define ISER_DEFAULT_SENDER_ADDRESS 0


namespace iser {

  struct DeviceParams {

    quint16   address         = ISER_DEFAULT_ADDRESS;
    quint16   sender_address  = ISER_DEFAULT_SENDER_ADDRESS;
    quint16   reset_interval  = ISER_DEFAULT_RESET_INTERVAL;

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

      P = P_ISER_ADDRESS;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.address = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Адрес должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        p.address = ISER_DEFAULT_ADDRESS;

      P = P_ISER_SENDER_ADDRESS;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.sender_address = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Адрес отправителя данных должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        p.sender_address = ISER_DEFAULT_SENDER_ADDRESS;


      P = P_ISER_RESET_TIMEOUT;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                                 .arg(P)
                                 .arg(object.value(P).toVariant().toString())
                                 .arg("Период сброса не может быть меньше 1 мсек."));

        p.reset_interval = object.value(P).toInt(ISER_DEFAULT_RESET_INTERVAL);

      }
      else
        p.reset_interval = ISER_DEFAULT_RESET_INTERVAL;


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
//      QString r;

//      r = QString::number(start_register, 16);

//      if(r.length() < 4)
//        r.push_front(QString(4 - r.length(), QChar('0')));

//      QString start_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);


//      r = QString::number(last_register, 16);

//      if(r.length() < 4)
//        r.push_front(QString(4 - r.length(), QChar('0')));

//      QString last_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);


//      j.insert(P_OPA_START_REGISTER, QJsonValue(start_r).toString());
//      j.insert(P_OPA_LAST_REGISTER, QJsonValue(last_r).toString());
      j.insert(P_ISER_RESET_TIMEOUT, QJsonValue(reset_interval).toInt(ISER_DEFAULT_RESET_INTERVAL));

      return j;

    }
  };
}

#endif // ISER_DEVICE_PARAMS

