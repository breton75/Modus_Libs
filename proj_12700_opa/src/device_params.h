#ifndef OPA_DEVICE_PARAMS
#define OPA_DEVICE_PARAMS

#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../svlib/sv_exception.h"

#define OPA_RESET_INTERVAL  10

// имена параметров устройств
#define P_OPA_START_REGISTER  "start_register"
#define P_OPA_LAST_REGISTER   "last_register"
#define P_OPA_RESET_TIMEOUT   "reset_timeout"

#define OPA_DEV_IMPERMISSIBLE_VALUE "Недопустимое значение параметра %1: %2.\n%3"
#define OPA_DEV_NO_PARAM  "Для устройства не задан обязательный параметр \"%1\""

namespace opa {

  struct DeviceParams {

    quint16   start_register = 0;
    quint16   last_register = 0;
    quint16   reset_interval = OPA_RESET_INTERVAL;

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

      P = P_OPA_START_REGISTER;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.start_register = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(OPA_DEV_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Стартовый регистр должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        throw SvException(QString(DEV_NO_PARAM).arg(P));

      P = P_OPA_LAST_REGISTER;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.last_register = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(OPA_DEV_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Конечный регистр должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        throw SvException(QString(DEV_NO_PARAM).arg(P));


      P = P_OPA_RESET_TIMEOUT;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(OPA_DEV_IMPERMISSIBLE_VALUE)
                                 .arg(P)
                                 .arg(object.value(P).toVariant().toString())
                                 .arg("Период сброса не может быть меньше 1 мсек."));

        p.reset_interval = object.value(P).toInt(OPA_RESET_INTERVAL);

      }
      else
        p.reset_interval = OPA_RESET_INTERVAL;


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

      r = QString::number(start_register, 16);

      if(r.length() < 4)
        r.push_front(QString(4 - r.length(), QChar('0')));

      QString start_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);


      r = QString::number(last_register, 16);

      if(r.length() < 4)
        r.push_front(QString(4 - r.length(), QChar('0')));

      QString last_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);


      j.insert(P_OPA_START_REGISTER, QJsonValue(start_r).toString());
      j.insert(P_OPA_LAST_REGISTER, QJsonValue(last_r).toString());
      j.insert(P_OPA_RESET_TIMEOUT, QJsonValue(reset_interval).toInt());

      return j;

    }
  };
}

#endif // DEVICEPARAMS

