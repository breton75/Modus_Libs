#ifndef DEVICE_PARAMS
#define DEVICE_PARAMS

#include <QtCore>
#include <QJsonDocument>

#include "conning_kongsber_device_global.h"

#include "../../../svlib/sv_exception.h"

#define RESET_INTERVAL  180 // миллисекунд

// имена параметров устройства
#define P_RESET_TIMEOUT   "reset_timeout"

#define CNKG_IMPERMISSIBLE_VALUE "Недопустимое значение параметра \"%1\": %2.\n%3"


  struct DeviceParams {

    quint16   reset_timeout = RESET_INTERVAL;

    bool isValid = true;

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

      P = P_RESET_TIMEOUT;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(CNKG_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Период сброса не может быть меньше 1 мсек."));

        p.reset_timeout = object.value(P).toInt(RESET_INTERVAL);

      }
      else p.reset_timeout = quint16(RESET_INTERVAL);

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

      j.insert(P_RESET_TIMEOUT, QJsonValue(reset_timeout));

      return j;

    }

  };


#endif // DEVICE_PARAMS

