/**********************************************************************
 *  автор Свиридов С.А. НИИ РПИ
 * *********************************************************************/

#ifndef TCP_IFC_DEFS
#define TCP_IFC_DEFS

#include <QtGlobal>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../svlib/SvException/svexception.h"
#include "../../../../Modus/global/global_defs.h"

#define P_TCP_MODE                  "mode"
#define P_TCP_IFC                   "ifc"
#define P_TCP_HOST                  "host"
#define P_TCP_PORT                  "port" // для клиента - порт подключения, для сервера - порт, который надо слушать
#define P_TCP_FMT                   "fmt"

#define P_MODE_CLIENT               "client"
#define P_MODE_SERVER               "server"

#define DEFAULT_PORT                5000

namespace tcp {

  /*** constants ***/
  enum ResponseAwaiting {
    DontWait = -1,
    Infinite = 0
  };

  const QMap<QAbstractSocket::SocketError, QString> SocketErrors = {
    {QAbstractSocket::ConnectionRefusedError,           "ConnectionRefusedError"          },
    {QAbstractSocket::RemoteHostClosedError,            "RemoteHostClosedError"           },
    {QAbstractSocket::HostNotFoundError,                "HostNotFoundError"               },
    {QAbstractSocket::SocketAccessError,                "SocketAccessError"               },
    {QAbstractSocket::SocketResourceError,              "SocketResourceError"             },
    {QAbstractSocket::SocketTimeoutError,               "SocketTimeoutError"              },/* 5 */
    {QAbstractSocket::DatagramTooLargeError,            "DatagramTooLargeError"           },
    {QAbstractSocket::NetworkError,                     "NetworkError"                    },
    {QAbstractSocket::AddressInUseError,                "AddressInUseError"               },
    {QAbstractSocket::SocketAddressNotAvailableError,   "SocketAddressNotAvailableError"  },
    {QAbstractSocket::UnsupportedSocketOperationError,  "UnsupportedSocketOperationError" },/* 10 */
    {QAbstractSocket::UnfinishedSocketOperationError,   "UnfinishedSocketOperationError"  },
    {QAbstractSocket::ProxyAuthenticationRequiredError, "ProxyAuthenticationRequiredError"},
    {QAbstractSocket::SslHandshakeFailedError,          "SslHandshakeFailedError"         },
    {QAbstractSocket::ProxyConnectionRefusedError,      "ProxyConnectionRefusedError"     },
    {QAbstractSocket::ProxyConnectionClosedError,       "ProxyConnectionClosedError"      },/* 15 */
    {QAbstractSocket::ProxyConnectionTimeoutError,      "ProxyConnectionTimeoutError"     },
    {QAbstractSocket::ProxyNotFoundError,               "ProxyNotFoundError"              },
    {QAbstractSocket::ProxyProtocolError,               "ProxyProtocolError"              },
    {QAbstractSocket::OperationError,                   "OperationError"                  },
    {QAbstractSocket::SslInternalError,                 "SslInternalError"                },/* 20 */
    {QAbstractSocket::SslInvalidUserDataError,          "SslInvalidUserDataError"         },
    {QAbstractSocket::TemporaryError,                   "TemporaryError"                  }
  };

  const QMap<QAbstractSocket::SocketState, QString> SocketStates = {
    {QAbstractSocket::UnconnectedState, "Нет подключения"},
    {QAbstractSocket::HostLookupState,  "Поиск узла" },
    {QAbstractSocket::ConnectingState,  "Подключение" },
    {QAbstractSocket::ConnectedState,   "Подключено"  },
    {QAbstractSocket::BoundState,       "Есть связь"      },
    {QAbstractSocket::ListeningState,   "Прослушивание"  },
    {QAbstractSocket::ClosingState,     "Закрытие соединения"    }
  };

  enum LogDataFlags {
    NoLog         = 0x0,
    LogInData     = 0x1,
    InDataAsHex   = 0x2,
    LogOutData    = 0x4,
    OutDataAsHex  = 0x8,
  };

  enum LogFormat {
    HEX,
    ASCII,
    DATALEN
  };

  #define DEFAULT_FLAGS tcp::LogInData | tcp::LogOutData

  const QMap<QString, QHostAddress::SpecialAddress> SpecialHosts = {{"localhost", QHostAddress::LocalHost},
                                                                    {"any",       QHostAddress::Any},
                                                                    {"broadcast", QHostAddress::Broadcast}};

  const QMap<QString, QHostAddress::SpecialAddress> LogFormats = {{"hex", HEX}, {"ascii", ASCII}, {"datalen", DATALEN}};

  /** структура для хранения параметров udp **/
  struct Params {

    QString      mode             = P_MODE_CLIENT;
    QString      ifc              = "";
    QHostAddress host             = QHostAddress::Any;
    quint16      port             = DEFAULT_PORT;
    quint16      timeout          = DEFAULT_TIMEOUT;
    quint16      fmt              = HEX;
    quint16      grain_gap        = DEFAULT_GRAIN_GAP;

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

      /* mode */
      P = P_TCP_MODE;
      if(object.contains(P)) {

        p.mode = object.value(P).toString(P_MODE_CLIENT).toLower();

        switch (qHash(p.mode)) {

          case qHash(P_MODE_CLIENT):

            break;

          default:
            throw SvException(QString(IMPERMISSIBLE_VALUE)
                               .arg(P).arg(object.value(P).toVariant().toString())
                               .arg(QString("Режим \"%1\" не поддерживается").arg(p.mode)));
            break;
        }
      }
      else
        p.mode = "client";

      /* ifc */
      P = P_TCP_IFC;
      p.ifc = object.contains(P) ? object.value(P).toString("") : "";

      /* host */
      P = P_TCP_HOST;
      if(object.contains(P)) {

        QString host = object.value(P).toString("").toLower();

        if(SpecialHosts.contains(host)) p.host = QHostAddress(SpecialHosts.value(host));

        else
          if(QHostAddress(host).toIPv4Address() != 0) p.host = QHostAddress(host);

        else
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(QJsonDocument(object).toJson(QJsonDocument::Compact))
                             .arg("Допускаются ip адреса в формате 192.168.1.1, а также \"localhost\", \"any\", \"broadcast\""));
      }
      else
        p.host = QHostAddress::Any;

      /* port */
      P = P_TCP_PORT;
      if(object.contains(P))
      {
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(QJsonDocument(object).toJson(QJsonDocument::Compact))
                             .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

        p.port = object.value(P).toInt(DEFAULT_PORT);

      }
      else
         throw SvException(QString(MISSING_PARAM_DESC)
                           .arg(QJsonDocument(object).toJson(QJsonDocument::Compact)).arg(P));


      /* timeout */
      P = P_TIMEOUT;
      if(object.contains(P))
      {
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(QJsonDocument(object).toJson(QJsonDocument::Compact))
                             .arg("Таймаут подключения должен быть задан целым положительным числом в миллисекундах"));

        p.timeout = object.value(P).toInt(DEFAULT_TIMEOUT);

      }
      else
        p.timeout = DEFAULT_TIMEOUT;

      /* log fmt */
      P = P_TCP_FMT;
      if(object.contains(P)) {

        if(!object.value(P).isString())
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P).arg(QJsonDocument(object).toJson(QJsonDocument::Compact))
                            .arg(QString("Формат вывода данных должен быть задан строковым значением [\"hex\"|\"ascii\"|\"datalen\"]")));

        QString fmt = object.value(P).toString(P_TCP_FMT).toLower();

        if(!LogFormats.contains(fmt))
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P).arg(QJsonDocument(object).toJson(QJsonDocument::Compact))
                            .arg(QString("Не поддерживаемый формат вывода данных. Допустимые значения: [\"hex\"|\"ascii\"|\"datalen\"]")));

        p.fmt = LogFormats.value(fmt);

      }
      else
        p.fmt = HEX;

      /* grain gap*/
      P = P_GRAIN_GAP;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(QJsonDocument(object).toJson(QJsonDocument::Compact))
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

      j.insert(P_TCP_IFC,   QJsonValue(ifc).toString());
      j.insert(P_TCP_HOST,  QJsonValue(host.toString()).toString());
      j.insert(P_TCP_PORT,  QJsonValue(static_cast<int>(port)).toInt());
      j.insert(P_TIMEOUT,   QJsonValue(static_cast<int>(timeout)).toInt());
      j.insert(P_TCP_FMT,   QJsonValue(static_cast<int>(fmt)).toInt());

      return j;

    }
  };
}


#endif // TCP_IFC_DEFS

