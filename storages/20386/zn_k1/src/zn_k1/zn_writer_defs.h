#ifndef ZN_K1_DEFS
#define ZN_K1_DEFS

/**********************************************************************
 *  автор Свиридов С.А. НИИ РПИ
 * *********************************************************************/

#include <QtGlobal>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../../../svlib/SvException/svexception.h"
#include "../../../../../../Modus/global/global_defs.h"

#define P_HOST            "host"
#define P_PORT            "port"
#define P_ZONE            "zone"
#define P_PASS            "pass"

#define DEFAULT_PORT      10000
#define DEFAULT_INTERVAL  1000

#define CMD_CONNECT       200
#define CMD_ANSWER        100
#define CMD_WRITE         501
#define ACC_CODE_WRITE    2


namespace zn1 {

  struct Params {

    QHostAddress host     = QHostAddress();
    quint16      port     = DEFAULT_PORT;
    quint16      interval = DEFAULT_INTERVAL;
    QString      zone     = QString();
    QString      pass     = QString();

    static Params fromJsonString(const QString& json_string) //throw (SvException)
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

    static Params fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      Params p;
      QString P;


      /* host */
      P = P_HOST;
      if(object.contains(P)) {

        QString host = object.value(P).toString("").toLower();

        if(QHostAddress(host).toIPv4Address() == 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Допускаются только ip адреса в формате 'xxx.xxx.xxx.xxx' (192.168.1.1)"));

        p.host = QHostAddress(host);

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      /* receive port */
      P = P_PORT;
      if(object.contains(P))
      {
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

        p.port = object.value(P).toInt(DEFAULT_PORT);

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      /* send port */
      P = P_INTERVAL;
      if(object.contains(P))
      {
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Интервал записи данных должен быть задан целым положительным числом"));

        p.interval = object.value(P).toInt(DEFAULT_INTERVAL);

      }
      else
        p.interval = DEFAULT_INTERVAL;

      P = P_ZONE;
      if(object.contains(P)) {

        p.zone = object.value(P).toString();

        if(p.zone.isEmpty())
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Имя зоны для записи не может быть пустым"));
      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      P = P_PASS;
      if(object.contains(P)) {

        p.pass = object.value(P).toString();

        if(p.pass.isEmpty())
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Пароль для подключения к накопителю не может быть пустым"));
      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));


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

      j.insert(P_HOST,     QJsonValue(host.toString()).toString());
      j.insert(P_PORT,     QJsonValue(static_cast<int>(port)).toInt());
      j.insert(P_INTERVAL, QJsonValue(static_cast<int>(interval)).toInt());

      return j;

    }
  };
}



#endif // ZN_K1_DEFS

