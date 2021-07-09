/**********************************************************************
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

#define DEFAULT_RECV_PORT 6001
#define DEFAULT_SEND_PORT 5001

const QMap<QString, QHostAddress::SpecialAddress> SpecialHosts = {{"localhost", QHostAddress::LocalHost},
                                                                  {"any",       QHostAddress::Any},
                                                                  {"broadcast", QHostAddress::Broadcast}};

/** структура для хранения параметров udp **/
struct UdpParams {

  QString      ifc              = "";
  QHostAddress host             = QHostAddress::Any;
  quint16      recv_port        = DEFAULT_RECV_PORT;
  quint16      send_port        = DEFAULT_SEND_PORT;
  quint16      grain_gap        = DEFAULT_GRAIN_GAP;

  static UdpParams fromJsonString(const QString& json_string) //throw (SvException)
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

  static UdpParams fromJsonObject(const QJsonObject &object) //throw (SvException)
  {
    UdpParams p;
    QString P;


    /* host */
    P = P_UDP_IFC;
    p.ifc = object.contains(P) ? object.value(P).toString("") : "";

    /* host */
    P = P_UDP_HOST;
    if(object.contains(P)) {

      QString host = object.value(P).toString("").toLower();

      if(SpecialHosts.contains(host)) p.host = QHostAddress(SpecialHosts.value(host));

      else
        if(QHostAddress(host).toIPv4Address() != 0) p.host = QHostAddress(host);

      else
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Допускаются ip адреса в формате 192.168.1.1, а также слова \"localhost\", \"any\", \"broadcast\""));
    }
    else
      p.host = QHostAddress::Any;

    /* receive port */
    P = P_UDP_RECV_PORT;
    if(object.contains(P))
    {
      if(object.value(P).toInt(-1) < 1)
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

      p.recv_port = object.value(P).toInt(DEFAULT_RECV_PORT);

    }
    else
      p.recv_port = DEFAULT_RECV_PORT;

    /* send port */
    P = P_UDP_SEND_PORT;
    if(object.contains(P))
    {
      if(object.value(P).toInt(-1) < 1)
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

      p.send_port = object.value(P).toInt(DEFAULT_SEND_PORT);

    }
    else
      p.send_port = DEFAULT_SEND_PORT;

    /* grain gap*/
    P = P_GRAIN_GAP;
    if(object.contains(P)) {

      if(object.value(P).toInt(-1) < 1)
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                          .arg(P)
                          .arg(object.value(P).toVariant().toString())
                          .arg("Интервал ожидания частей пакета не может быть меньше 1 мсек."));

      p.grain_gap = object.value(P).toInt(DEFAULT_GRAIN_GAP);

    }
    else p.grain_gap = quint16(DEFAULT_GRAIN_GAP);


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

    return j;

  }
};


#endif // UDP_IFC_DEFS

