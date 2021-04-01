#ifndef RADUGA_PROTOCOL_PARAMS
#define RADUGA_PROTOCOL_PARAMS

#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../../svlib/sv_exception.h"
#include "../../../../../Modus/global/global_defs.h"
//#include "../../../../../Modus/global/device/device_defs.h"

// имена параметров
#define P_START_REGISTER  "start_register"
#define P_LAST_REGISTER   "last_register"

namespace raduga {

  struct ProtocolParams {

    quint16   start_register = 0;
    quint16   last_register = 0;

    static ProtocolParams fromJson(const QString& json_string) //throw (SvException)
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

    static ProtocolParams fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      ProtocolParams p;
      QString P;

      P = P_START_REGISTER;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.start_register = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Стартовый регистр должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      P = P_LAST_REGISTER;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.last_register = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Конечный регистр должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

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


      j.insert(P_START_REGISTER, QJsonValue(start_r).toString());
      j.insert(P_LAST_REGISTER, QJsonValue(last_r).toString());

      return j;

    }
  };
}

#endif // RADUGA_PROTOCOL_PARAMS

