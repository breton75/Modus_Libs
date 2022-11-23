#ifndef TCP_SERVER_DEFS
#define TCP_SERVER_DEFS

#include <QtGlobal>
#include <QHostAddress>
#include <QNetworkInterface>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../svlib/SvException/svexception.h"
#include "../../../../Modus/global/global_defs.h"

// Параметр "listen_address" в конфигурационном файле определяет адрес, который сервер должен прослушивать:
#define P_TCP_LISTEN_ADDRESS   "listen_address"

// определяет имя сетвого интерфейса, который сервер должен прослушивать:
#define P_IFC_NAME   "ifc"

// Параметр "port" в конфигурационном файле определяет адрес порта, который сервер должен прослушивать:
#define P_TCP_PORT   "port"

// Параметр "grain_gap" в конфигурационном файле определяет период таймера, используемого при чтении
// данных от клиента. Подробное описание - см. функцию "SvTcp::read".
#define P_GRAIN_GAP  "grain_gap"

// Параметр "fmt" в конфигурационном файле определяет формат данных в сообщениях
// отображаемых в утилите "logview":
#define P_TCP_FMT    "fmt"

// Значение параметра "port" по умолчанию (то есть если оно не задано в конфигурационном файле):
#define DEFAULT_PORT                 0

// Значение периода таймера, используемого при чтении данных от клиента, по умолчанию (то есть
// если оно не задано в конфигурационном файле):
#define DEFAULT_GRAIN_GAP           10

#define DEFAULT_IFC_NAME            "any"

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

  /** структура для хранения параметров протокольной части **/
  struct Params {
    // Адрес, который сервер должен прослушивать TCP-сервер.
    // также может быть определен по имени сетевого интерфейса, для которого запускается сервер.
    // совместное использование этих параметров не имеет смысла: если они оба определены в
    // файле конфигруации, то будет использоваться значение параметра listen_address
    // В случае, если для интрефейса задано несколько ip адресов, то будет использован первый
    QHostAddress listen_address = QHostAddress::Any;
    QString      ifc_name       =  DEFAULT_IFC_NAME;

    // Порт, который должен прослушивать TCP-сервер:
    quint16      port           = DEFAULT_PORT;

    // Формат данных в сообщениях, отображаемых в утилите "logview":
    quint16      fmt            = modus::HEX;

    // Период таймера, используемого при чтении данных от клиента.
    // Подробное описание - см. функцию "SvTcp::read"
    quint16      grain_gap      = DEFAULT_GRAIN_GAP;

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

      // Считываем значение параметра "интерфейс, который сервер должен прослушивать":
      P = P_IFC_NAME;
      if(object.contains(P)) {

        if(!object.value(P).isString())
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
                            .arg(QString("Имя интерфейса должно быть задано строковым значением.")));

        p.ifc_name = object.value(P).toString("");

        if(p.ifc_name.toLower() == DEFAULT_IFC_NAME)
          p.listen_address = QHostAddress::Any;

        else {

          QNetworkInterface ifc = QNetworkInterface::interfaceFromName(p.ifc_name);
          if(!ifc.isValid())
            throw SvException(QString("Задано неверное имя интерфейса: %1").arg(p.ifc_name));

          if(ifc.addressEntries().count() == 0)
            throw SvException(QString("Сетевой интерфейс %1 не имеет назначенных ip адресов").arg(p.ifc_name));

          p.listen_address = ifc.addressEntries().first().ip();

        }
      }
      else {
        // в создании соединения используется только параметр listen_address
        p.ifc_name = DEFAULT_IFC_NAME;
        p.listen_address = QHostAddress::Any;
      }

      // Считываем значение параметра "адрес, который сервер должен прослушивать":
      P = P_TCP_LISTEN_ADDRESS;
      if(object.contains(P)) {

        QString listen_address = object.value(P).toString("").toLower();

        if(SpecialHosts.contains(listen_address)) p.listen_address = QHostAddress(SpecialHosts.value(listen_address));

        else
          if(QHostAddress(listen_address).toIPv4Address() != 0) p.listen_address = QHostAddress(listen_address);

        else
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
                             .arg("Допускаются ip адреса в формате 192.168.1.1, а также \"localhost\", \"any\", \"broadcast\""));
      }
      else
        p.listen_address = QHostAddress::Any;

      // Считываем значение параметра "порт, который сервер должен прослушивать"
      P = P_TCP_PORT;
      if(object.contains(P))
      {
        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
                             .arg("Номер порта должен быть задан целым положительным числом в диапазоне [1..65535]"));

        p.port = object.value(P).toInt(DEFAULT_PORT);

      }
      else
         throw SvException(QString(MISSING_PARAM_DESC)
                           .arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact))).arg(P));

      // Считываем значение параметра "формат данных в сообщениях, отображаемых в утилите "logview":
      P = P_TCP_FMT;
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

      // Считываем значение параметра "период таймера, используемого при чтении данных от клиента":
      P = P_GRAIN_GAP;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 1)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
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

        j.insert(P_TCP_LISTEN_ADDRESS,        QJsonValue(listen_address.toString()).toString());
        j.insert(P_TCP_PORT,        QJsonValue(static_cast<int>(port)).toInt());
        j.insert(P_GRAIN_GAP,       QJsonValue(grain_gap));
        j.insert(P_TCP_FMT,         QJsonValue(static_cast<int>(fmt)).toInt());

        return j;
    }
  };
}


#endif // TCP_SERVER_DEFS

