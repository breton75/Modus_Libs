﻿#ifndef RADUGA_PROTOCOL_PARAMS
#define RADUGA_PROTOCOL_PARAMS

#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../../../svlib/SvException/svexception.h"
#include "../../../../../../Modus/global/global_defs.h"
//#include "../../../../../Modus/global/device/device_defs.h"

#define P_ABONENT  "abonent"

namespace raduga {

  struct ProtocolParams {

    quint16 packid         = 0;
    quint16 abonent        = 0;
    quint16 parse_interval = 1000;

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

      P = P_PACKID;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Идентификатор пакета должен быть задан двухбайтным целым числом"));

        p.packid = object.value(P).toInt();

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      P = P_ABONENT;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                                 .arg(P)
                                 .arg(object.value(P).toVariant().toString())
                                 .arg("Идентификатор абонента должен быть задан двухбайтным целым числом"));

        p.abonent = object.value(P).toInt();

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      P = P_PARSE_INTERVAL;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Интервал разбора данных должен быть задан двухбайтным целым числом в миллисекундах"));

        p.parse_interval = object.value(P).toInt();

      }
      else
        p.parse_interval = 1000;

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

      j.insert(P_PACKID,          QJsonValue(packid).toString());
      j.insert(P_ABONENT,         QJsonValue(abonent).toInt());
      j.insert(P_PARSE_INTERVAL,  QJsonValue(parse_interval).toInt());

      return j;

    }
  };
}

#endif // RADUGA_PROTOCOL_PARAMS

