#ifndef ZN_PARAMS
#define ZN_PARAMS

#include <QString>
#include <QtCore>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../../svlib/SvException/svexception.h"
#include "../../../../../Modus/global/global_defs.h"
//#include "../../../../../Modus/global/device/device_defs.h"

#define P_ZN_MARKER     "zn_marker"

namespace zn1 {

  struct SignalParams {

    QString zn_marker = "";

    static SignalParams fromJson(const QString& json_string) //throw (SvException)
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

    static SignalParams fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      SignalParams p;
      QString P;

      P = P_ZN_MARKER;
      if(object.contains(P)) {

        if(!object.value(P).isString())
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Маркер записи должен быть задан строкой."));

        p.zn_marker = object.value(P).toString();

      }
      else
        throw SvException(QString(MISSING_PARAM_DESC).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact))).arg(P));

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

      j.insert(P_ZN_MARKER,  QJsonValue(zn_marker).toString());

      return j;

    }
  };
}

#endif // ZN_PARAMS

