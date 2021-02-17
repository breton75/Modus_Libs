/**********************************************************************
 *  автор Свиридов С.А. Авиационные и Морская Электроника
 * *********************************************************************/

#ifndef CAN_PARAMS
#define CAN_PARAMS

#include <QtGlobal>
#include <QVariant>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../Modus/global/misc/sv_exception.h"
#include "../../../../Modus/global/global_defs.h"

// имена параметров для CAN
#define P_PORTNAME "portname"


struct CANParams {

  QString                   portname    =     "can0";

  bool isValid = true;

  static CANParams fromJsonString(const QString& json_string) //throw (SvException)
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

  static CANParams fromJsonObject(const QJsonObject &object) //throw (SvException)
  {
    CANParams p;
    QString P;

    P = P_PORTNAME;
    if(object.contains(P)) {

      p.portname = object.value(P).toString();

      if(p.portname.isEmpty())
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Имя порта не может быть пустым"));
    }
    else
      throw SvException(QString(IMPERMISSIBLE_VALUE)
                         .arg(P).arg(object.value(P).toVariant().toString())
                         .arg("Должно быть задано имя порта"));

    return p;

  }

  QString toJsonString(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const
  {
    QJsonDocument jd;
    jd.setObject(toJsonObject());

    return QString(jd.toJson(format));
  }

  QJsonObject toJsonObject() const
  {
    QJsonObject j;

    j.insert(P_PORTNAME, QJsonValue(portname).toString());

    return j;

  }
};


#endif // CAN_PARAMS

