/**********************************************************************
 *  автор Свиридов С.А. Авиационные и Морская Электроника
 * *********************************************************************/

#ifndef CAN_IFC_PARAMS
#define CAN_IFC_PARAMS

#include <QtGlobal>
#include <QVariant>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../svlib/SvException/svexception.h"
#include "../../../../Modus/global/global_defs.h"

// имена параметров для CAN
#define P_PORTNAME "portname"
#define P_BITRATE  "bitrate"
#define P_LOG_FMT  "fmt"

#define DEFAULT_CAN_BITRATE 125000

struct CANParams {

  QString         portname    = "can0";
  int             bitrate     = DEFAULT_CAN_BITRATE;
  quint16         fmt         = modus::HEX;

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

    // log fmt
    P = P_LOG_FMT;
    if(object.contains(P)) {

      if(!object.value(P).isString())
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                          .arg(P).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
                          .arg(QString("Формат вывода данных должен быть задан строковым значением [\"hex\"|\"ascii\"|\"len\"]")));

      QString fmt = object.value(P).toString("hex").toLower();

      if(!modus::LogFormats.contains(fmt))
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                          .arg(P).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
                          .arg(QString("Не поддерживаемый формат вывода данных. Допустимые значения: [\"hex\"|\"ascii\"|\"len\"]")));

      p.fmt = modus::LogFormats.value(fmt);

    }
    else
      p.fmt = modus::HEX;

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


#endif // CAN_IFC_PARAMS

