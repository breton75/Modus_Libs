﻿/**********************************************************************
 *  автор Свиридов С.А. НИИ РПИ
 * *********************************************************************/

#ifndef UDP_IFC_DEFS
#define UDP_IFC_DEFS

#include <QtGlobal>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../svlib/SvException/svexception.h"
#include "../../../../Modus/global/global_defs.h"

#define P_UDP_IFC                   "ifc"
#define P_UDP_HOST                  "host"
#define P_UDP_RECV_PORT             "recv_port"
#define P_UDP_SEND_PORT             "send_port"
#define P_UDP_FMT                   "fmt"
#define P_UDP_FORWARDING            "forwarding"

#define P_UDP_IFC_DESC              "имя сетевого интерфейса для обмена данными"
#define P_UDP_HOST_DESC             "ip адрес узла, с которым производится обмен данными"
#define P_UDP_RECV_PORT_DESC        "порт для приема входящхх данных"
#define P_UDP_SEND_PORT_DESC        "порт удаленного узла, на который отправляются данные"
#define P_GRAIN_GAP_DESC            "интервал (в милисекундах) ожидания частей пакета данных"
#define P_UDP_FORWARDING_DESC       "параметры перенаправления данных. ip адрес узла и порт"
#define P_FMT_DESC                  "форматирование сообщений для логирования"


#define DEFAULT_RECV_PORT 6001
#define DEFAULT_SEND_PORT 5001

const QMap<QString, QHostAddress::SpecialAddress> SpecialHosts = {{"localhost", QHostAddress::LocalHost},
                                                                  {"any",       QHostAddress::Any},
                                                                  {"broadcast", QHostAddress::Broadcast}};

namespace udp {

  const char* usage = "{\"params\": [\n"
      MAKE_PARAM_STR_2(P_UDP_IFC,         P_UDP_IFC_DESC,         "string",   "false",  "",                       "", ",\n")\
      MAKE_PARAM_STR_2(P_UDP_HOST,        P_UDP_HOST_DESC,        "string",   "false",  "any",                    "ip адреса в формате xxx.xxx.xxx.xxx, localhost, any", ",\n")\
      MAKE_PARAM_STR_2(P_UDP_RECV_PORT,   P_UDP_RECV_PORT_DESC,   "quint16",  "true",   "",                       "1 - 65535", ",\n")\
      MAKE_PARAM_STR_2(P_UDP_SEND_PORT,   P_UDP_SEND_PORT_DESC,   "quint16",  "false",  "равен " P_UDP_RECV_PORT, "1 - 65535", ",\n")\
      MAKE_PARAM_STR_2(P_FMT,             P_FMT_DESC,             "string",   "false",  "hex",                    "hex | ascii | len", ",\n")\
      MAKE_PARAM_STR_2(P_GRAIN_GAP,       P_GRAIN_GAP_DESC,       "quint16",  "false",  "10",                     "1 - 65535", ",\n")\
      MAKE_PARAM_STR_2(P_UDP_FORWARDING,  P_UDP_FORWARDING_DESC,  "string",   "false",  "",                       "json объект вида {\\\"host\\\": \\\"xxx.xxx.xxx.xxx\\\", \\\"port\\\": xxx}", "\n")\
      "]}";

  /** структура для хранения параметров udp **/
  struct Params {

    QString      ifc              = "";
    QHostAddress host             = QHostAddress::Any;
    quint16      recv_port        = DEFAULT_RECV_PORT;
    quint16      send_port        = DEFAULT_SEND_PORT;
    quint16      fmt              = modus::HEX;
    quint16      grain_gap        = DEFAULT_GRAIN_GAP;
    QHostAddress forward_host     = QHostAddress::Any;
    quint16      forward_port     = DEFAULT_RECV_PORT;

    static udp::Params fromJsonString(const QString& json_string) //throw (SvException)
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

    static udp::Params fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      udp::Params p;
      QString P;
      QString json = QString(QJsonDocument(object).toJson(QJsonDocument::Compact));

      /* host */
      P = P_UDP_IFC;
      p.ifc = object.contains(P) ? object.value(P).toString("") : "";

      /* host */
      P = P_UDP_HOST;
      if(object.contains(P)) {

        QString host = object.value(P).toString("").toLower();

        if(SpecialHosts.contains(host))
          p.host = QHostAddress(SpecialHosts.value(host));

        else
          if(QHostAddress(host).toIPv4Address() != 0) p.host = QHostAddress(host);

        else
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                             .arg("Допускаются ip адреса в формате 192.168.1.1, а также слова \"localhost\", \"any\", \"broadcast\""));
      }
      else
        p.host = QHostAddress::Any;

      /* receive port */
      P = P_UDP_RECV_PORT;
      if(object.contains(P))
      {
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                             .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

        p.recv_port = object.value(P).toInt();

      }
      else
        throw SvException(QString(MISSING_PARAM_DESC).arg(json).arg(P));
  //      p.recv_port = DEFAULT_RECV_PORT;

      /* send port */
      P = P_UDP_SEND_PORT;
      if(object.contains(P))
      {
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                             .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

        p.send_port = object.value(P).toInt();

      }
      else
        p.send_port = p.recv_port;

      /* log fmt */
      P = P_UDP_FMT;
      if(object.contains(P)) {

        if(!object.value(P).isString())
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                            .arg(QString("Формат вывода данных должен быть задан строковым значением [\"hex\"|\"ascii\"|\"len\"]")));

        QString fmt = object.value(P).toString("hex").toLower();

        if(!modus::LogFormats.contains(fmt))
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                            .arg(QString("Не поддерживаемый формат вывода данных. Допустимые значения: [\"hex\"|\"ascii\"|\"len\"]")));

        p.fmt = modus::LogFormats.value(fmt);

      }
      else
        p.fmt = modus::HEX;

      /* grain gap*/
      P = P_GRAIN_GAP;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                            .arg("Интервал ожидания частей пакета не может быть меньше 1 мсек."));

        p.grain_gap = object.value(P).toInt(DEFAULT_GRAIN_GAP);

      }
      else
        p.grain_gap = quint16(DEFAULT_GRAIN_GAP);

      // forwarding
      P = P_UDP_FORWARDING;
      if(object.contains(P)) {

        if(!object.value(P).isObject())
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                            .arg(QString("Параметры перенаправления (forwarding) должны быть заданы в виде json объекта")));

        QJsonObject fo = object.value(P).toObject();

        // forward host
        P = P_UDP_HOST;
        if(fo.contains(P)) {

          QString fw_host = fo.value(P).toString("").toLower();

          if(SpecialHosts.contains(fw_host))
            p.forward_host = QHostAddress(SpecialHosts.value(fw_host));

          else
            if(QHostAddress(fw_host).toIPv4Address() != 0)
              p.forward_host = QHostAddress(fw_host);

          else
            throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                               .arg("Допускаются ip адреса в формате 192.168.1.1, а также слова \"localhost\", \"any\", \"broadcast\""));
        }
        else
          p.forward_host = QHostAddress::Any;

        // forward port
        P = P_PORT;
        if(fo.contains(P))
        {
          if(fo.value(P).toInt(-1) < 1)
            throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                               .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

          p.forward_port = fo.value(P).toInt(p.recv_port);

        }
        else
          p.forward_port = p.recv_port;

      }
      else {

        p.forward_host = QHostAddress();
        p.forward_port = 0;

      }

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

      j.insert(P_UDP_IFC,               QJsonValue(ifc).toString());
      j.insert(P_UDP_HOST,              QJsonValue(host.toString()).toString());
      j.insert(P_UDP_RECV_PORT,         QJsonValue(static_cast<int>(recv_port)).toInt());
      j.insert(P_UDP_SEND_PORT,         QJsonValue(static_cast<int>(send_port)).toInt());

      if(forward_port) {

        QJsonObject o;

        if(!forward_host.isNull())
          o.insert(P_UDP_HOST,          QJsonValue(forward_host.toString()).toString());

        j.insert(P_PORT,                QJsonValue(static_cast<int>(forward_port)).toInt());

        j.insert(P_UDP_FORWARDING,      QJsonValue(o).toObject());

      }

      return j;

    }
  };

}


#endif // UDP_IFC_DEFS

