﻿#ifndef TCP_CLIENT_DEFS
#define TCP_CLIENT_DEFS

#include <QtGlobal>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../svlib/SvException/svexception.h"
#include "../../../../Modus/global/global_defs.h"

#define P_HOST                    "host"
#define P_RECONNECT_PERIOD        "reconnect_period"
#define P_CONNECTIONS             "connections"

#define P_HOST_DESC               "ip адрес, к которому клиент должен подключаться"
#define P_TCP_PORT_DESC           "порт, к которому клиент должен подключаться"
#define P_RECONNECT_PERIOD_DESC   "период в милисекундах, с которым TCP-клиент осуществляет попытки установить соединение с сервером"
#define P_GRAIN_GAP_DESC          "период (в милисекундах) ожидания частей пакета данных"
#define P_FMT_DESC                "форматирование сообщений для логирования"

// дефолтное значение параметра "port" по умолчанию (то есть если оно не задано в конфигурационном файле):
#define DEFAULT_PORT              3004

// дефолтное значение периода с которым TCP-клиент осуществляет попытки установить соединение с сервером,
#define DEFAULT_RECONNECT_PERIOD  1000


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

  #define DEFAULT_FLAGS tcp::LogInData | tcp::LogOutData

  const QMap<QString, QHostAddress::SpecialAddress> SpecialHosts = {{"localhost", QHostAddress::LocalHost},
                                                                    {"any",       QHostAddress::Any},
                                                                    {"broadcast", QHostAddress::Broadcast}};

    // Cтруктура для хранения параметров TCP клиента:
  struct Params
  {
    // Адрес, к которому должен подключаться клиент:
    QHostAddress server_address   = QHostAddress::Null;

    // Порт, к которому должен подключаться клиент:
    quint16      port             = DEFAULT_PORT;

    // Формат данных в сообщениях, отображаемых в утилите "logview":
    quint16      fmt              = modus::HEX;

    // Период таймера, используемого при чтении данных от клиента.
    // Подробное описание - см. функцию "SvTcpClient::read"
    quint16      grain_gap        = DEFAULT_GRAIN_GAP;

    // Период с которым TCP-клиент осуществляет попытки установить соединение с сервером:
    quint16     reconnect_period  = DEFAULT_RECONNECT_PERIOD;

    static QString usage()
    {
      QString fmts = QString();
      for(auto key: modus::LogFormats.keys())
        fmts.append(key);

      QString result = QString("\"params\": [\n")
        .append(MAKE_PARAM_STR(P_HOST,              P_HOST_DESC,              "string",   "true", "NULL",                     "ip адреса в формате xxx.xxx.xxx.xxx, localhost", ",\n"))
        .append(MAKE_PARAM_STR(P_PORT,              P_TCP_PORT_DESC,          "quint16",  "false", DEFAULT_PORT,              "1 - 65535", ",\n"))
        .append(MAKE_PARAM_STR(P_RECONNECT_PERIOD,  P_RECONNECT_PERIOD_DESC,  "quint16",  "false", DEFAULT_RECONNECT_PERIOD,  "1 - 65535", ",\n"))
        .append(MAKE_PARAM_STR(P_GRAIN_GAP,         P_GRAIN_GAP_DESC,         "quint16",  "false", DEFAULT_GRAIN_GAP,         "1 - 65535", ",\n"))
        .append(MAKE_PARAM_STR(P_FMT,               P_FMT_DESC,               "string",   "false", "hex",                     fmts, "\n"))
        .append("]");

      return result;

    }

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
      QString json = QString(QJsonDocument(object).toJson(QJsonDocument::Compact));

      // Считываем значение параметра: "адрес к которому должен подключаться клиент":
      P = P_HOST;
      if(object.contains(P)) {

        QString server_address = object.value(P).toString("").toLower();

        if(SpecialHosts.contains(server_address)) p.server_address = QHostAddress(SpecialHosts.value(server_address));

        else
          if(QHostAddress(server_address).toIPv4Address() != 0) p.server_address = QHostAddress(server_address);

        else
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
                             .arg("Допускаются ip адреса в формате 192.168.1.1, а также \"localhost\", \"any\", \"broadcast\""));
      }
      else
        throw SvException(QString(MISSING_PARAM_DESC).arg(json).arg(P));

      // Считываем значение параметра: "порт, к которому должен подключаться клиент":
      P = P_PORT;
      if(object.contains(P))
      {
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                             .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

        p.port = object.value(P).toInt(DEFAULT_PORT);

      }
      else
         throw SvException(QString(MISSING_PARAM_DESC).arg(json).arg(P));


      // Считываем значение параметра "формат данных в сообщениях, отображаемых в утилите "logview":
      P = P_FMT;
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

     // Считываем значение параметра "период таймера, используемого при чтении данных от клиента":
      P = P_GRAIN_GAP;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                            .arg("Интервал ожидания частей пакета не может быть меньше 1 мсек."));

        p.grain_gap = object.value(P).toInt(DEFAULT_GRAIN_GAP);

      }
      else
        p.grain_gap = quint16(DEFAULT_GRAIN_GAP);

      // Считываем значение параметра "период с которым TCP-клиент осуществляет попытки установить
      // соединение с сервером":
      P = P_RECONNECT_PERIOD;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE).arg(P).arg(json)
                            .arg("Период в милисекундах, с которым TCP-клиент осуществляет попытки установить соединение с сервером, должен быть задан целым числом"));

        p.reconnect_period = object.value(P).toInt(DEFAULT_RECONNECT_PERIOD);

      }
      else p.reconnect_period = quint16(DEFAULT_RECONNECT_PERIOD);

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

      j.insert(P_HOST,              QJsonValue(server_address.toString()).toString());
      j.insert(P_PORT,              QJsonValue(port));
      j.insert(P_GRAIN_GAP,         QJsonValue(grain_gap));
      j.insert(P_FMT,               QJsonValue(fmt));
      j.insert(P_RECONNECT_PERIOD,  QJsonValue(reconnect_period));

      return j;
    }
  };
}


#endif // TCP_CLIENT_DEFS

