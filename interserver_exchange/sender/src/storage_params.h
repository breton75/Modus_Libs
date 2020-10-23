#ifndef ISES_DEVICE_PARAMS
#define ISES_DEVICE_PARAMS

#include <QString>
#include <QtCore>
#include <QUdpSocket>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../global/ise_defs.h"
#include "../../../../svlib/sv_exception.h"


namespace ises {

  const QMap<QString, QHostAddress::SpecialAddress> SpecialHosts = {{"localhost", QHostAddress::LocalHost},
                                                                  {"broadcast", QHostAddress::Broadcast}};

  struct StorageParams {

    quint16       iseid         = ISE_DEFAULT_ISEID;
    quint16       receiver      = ISE_DEFAULT_RECEIVER_ISEID;
    QHostAddress  host          = QHostAddress::Any;
    quint16       port          = ISE_DEFAULT_PORT;
    quint16       send_interval = ISE_DEFAULT_SEND_INTERVAL;

    static StorageParams fromJson(const QString& json_string) throw (SvException)
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

    static StorageParams fromJsonObject(const QJsonObject &object) throw (SvException)
    {
      StorageParams p;
      QString P;

      /* host */
      P = P_ISE_HOST;
      if(object.contains(P)) {

        QString host = object.value(P).toString("").toLower();

        if(SpecialHosts.contains(host)) p.host = QHostAddress(SpecialHosts.value(host));

        else
        if(QHostAddress(host).toIPv4Address() != 0)
          p.host = QHostAddress(host);

        else
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Допускаются ip адреса в формате \"192.168.1.1\", а также слова \"localhost\", \"any\", \"broadcast\""));
      }
      else
        p.host = QHostAddress::Any;


      /* port */
      P = P_ISE_PORT;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

        p.port = object.value(P).toInt(DEFAULT_RECV_PORT);

      }
      else
        p.port = ISE_DEFAULT_PORT;

      /* iseid */
      P = P_ISE_ISEID;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.iseid = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Идентификатор должен быть задан двухбайтовым целым числом в шестнадцатиричном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        p.iseid = ISE_DEFAULT_ISEID;

      /* receiver */
      P = P_ISE_RECEIVER_ISEID;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.receiver = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Идентификатор получателя данных должен быть задан двухбайтовым целым числом в шестнадцатиричном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

      }
      else
        p.receiver = ISE_DEFAULT_RECEIVER_ISEID;


      /* send_interval */
      P = P_ISE_SEND_INTERVAL;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                                 .arg(P)
                                 .arg(object.value(P).toVariant().toString())
                                 .arg("Интервал отправки должен быть задан целым числом и не может быть меньше 1 мсек."));

        p.send_interval = object.value(P).toInt(ISE_DEFAULT_SEND_INTERVAL);

      }
      else
        p.send_interval = ISE_DEFAULT_SEND_INTERVAL;


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

      r = QString::number(address, 16);
      QString iseid_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);

      r = QString::number(receiver, 16);
      QString receiver_iseid_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);

      j.insert(P_ISE_ISEID,           QJsonValue(iseid_r).toString("0x00"));
      j.insert(P_ISE_RECEIVER_ISEID,  QJsonValue(receiver_iseid_r).toString("0x00"));
      j.insert(P_ISE_HOST,            QJsonValue(host.toString()).toString("any"));
      j.insert(P_ISE_PORT,            QJsonValue(static_cast<int>(port)).toInt(ISE_DEFAULT_PORT));
      j.insert(P_ISE_SEND_INTERVAL,   QJsonValue(static_cast<int>(send_interval)).toInt(ISE_DEFAULT_SEND_INTERVAL));

      return j;

    }
  };
}

#endif // ISES_DEVICE_PARAMS

