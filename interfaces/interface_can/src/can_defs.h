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
#define P_BITRATE  "bitrate"

#define DEFAULT_CAN_BITRATE 125000

struct CANParams {

  QString                   portname    =     "can0";
  int                       bitrate     =     DEFAULT_CAN_BITRATE;

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
      throw SvException(QString(MISSING_PARAM).arg(QString("%1/%2").arg(P_INTERFACE).arg(P_PARAMS)).arg(P));


    // offset
    P = P_BITRATE;
    if(object.contains(P)) {

      if(object.value(P).toInt(-1) < 0)
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Параметр 'offset' должен быть задан целым положительным числом"));

      p.bitrate = object.value(P).toInt();

    }
    else
      throw SvException(QString(MISSING_PARAM).arg(QString("%1/%2").arg(P_INTERFACE).arg(P_PARAMS)).arg(P));


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
    j.insert(P_BITRATE,  QJsonValue(bitrate).toInt());

    return j;

  }
};


#endif // CAN_PARAMS

