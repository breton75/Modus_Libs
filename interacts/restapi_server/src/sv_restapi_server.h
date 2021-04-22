#ifndef SV_RESTAPI_SERVER_H
#define SV_RESTAPI_SERVER_H

//#include "QtWebSockets/QTcpSocketserver.h"
//#include "QtWebSockets/QTcpSocket.h"

#include <QTcpServer>
#include <QTcpSocket>

#include <QMap>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QHttpMultiPart>
#include <QDir>
#include <QHash>
#include <QFileInfo>
#include <QThreadPool>
#include <QByteArray>
#include <QDataStream>
#include <QCryptographicHash>

#include "restapi_server_global.h"

#include "../../../../Modus/global/interact/sv_abstract_interact.h"
#include "../../../../Modus/global/global_defs.h"
#include "../../../../Modus/global/configuration.h"

#include "restapi_server_defs.h"

extern "C" {

    INTERACT_SHARED_EXPORT modus::SvAbstractInteract* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

const QList<QString> Http_Field_List = QList<QString>() << "date"
                                                        << "content-type"
                                                        << "content-length"
                                                        << "last-modified"
                                                        << "connection"
                                                        << "upgrade"
                                                        << "origin"
                                                        << "host"
                                                        << "sec-websocket-key"
                                                        << "sec-websocket-version"
                                                        << "sec-websocket-accept"
                                                        << "sec-websocket-protocol";

namespace restapi {

  typedef QMap<QString, QString> HttpFields;

  struct HttpFld {

  };

  struct HttpRequest {

    QString     method;
    QString     resourse;
    QString     params;
    HttpFields  fields;
    QString     protocol;
    QString     version;
    QByteArray  data;

  };

  struct HttpReply {

    explicit HttpReply()
    {
      this->data      = QByteArray();
      this->protocol  = "HTTP";
      this->version   = "1.0";
      this->code      = 200;
      this->status    = "Ok";
    }

    explicit HttpReply(const QByteArray data, const QString protocol = "HTTP", const QString version = "1.0",
                       const int code = 200, const QString status = "Ok")
    {
      this->data      = data;
      this->protocol  = protocol;
      this->version   = version;
      this->code      = code;
      this->status    = status;
    }

    QString protocol;
    QString version;
    int     code;
    QString status;
    QByteArray data;

  };

  const QMap<QString, QString> ContentTypeBySuffix = {{"html", "text/html"},
                                                      {"cmd",  "text/cmd"},
                                                      {"css",  "text/css"},
                                                      {"csv",  "text/csv"},
                                                      {"txt",  "text/plain"},
                                                      {"php",  "text/php"},
                                                      {"ico",  "image/vnd.microsoft.icon"},
                                                      {"gif",  "image/gif"},
                                                      {"jpeg", "image/jpeg"},
                                                      {"png",  "image/png"},
                                                      {"svg",  "image/svg+xml"},
                                                      {"js",   "application/javascript"},
                                                      {"xml",  "application/xml"},
                                                      {"zip",  "application/zip"},
                                                      {"gzip", "application/gzip"},
                                                      {"pdf",  "application/pdf"},
                                                      {"json", "application/json"}
                                                     };
  class SvRestAPI;

}

class restapi::SvRestAPI: public modus::SvAbstractInteract
{
  Q_OBJECT

public:
  explicit SvRestAPI();
  ~SvRestAPI();

  bool configure(modus::InteractConfig* config, modus::Configuration* configuration) override;

  void start() override;
  void stop() override;

  bool bindSignal(modus::SvSignal* signal) override;

  const QMap<int, modus::SvSignal*>*      signalsById()   const { return &m_signals_by_id;   }
  const QHash<QString, modus::SvSignal*>* signalsByName() const { return &m_signals_by_name; }

private:
  QTcpServer* m_server;
  QList<QTcpSocket*> m_websocket_clients;

  restapi::Params m_params;

  QMap<int, modus::SvSignal*>      m_signals_by_id;
  QHash<QString, modus::SvSignal*> m_signals_by_name;

  bool m_is_active;
  bool m_is_websocket = false;

  QByteArray reply_http_get(const HttpRequest &request);
  QByteArray reply_http_get_params(const HttpRequest &request);
  QByteArray reply_http_post(const HttpRequest &request);
  QByteArray reply_ws_get(const HttpRequest &request);

  QByteArray getEntityData(const QString& entity, const QString& matter, const QString& option, const QString& list, const char separator = ',');

  QByteArray getSignalsData(const QString& what, const QString& option, const QString& list, const char separator = ',');
  QByteArray getSignalsValues(const QString& option, const QString& list, const char separator);

  QByteArray getConfigurationData(const QString& what, const QString& option, const QString& list, const char separator = ',');

  QByteArray getHttpError(int errorCode, QString errorString);

private slots:
  void newConnection();
  void processHttpRequest();
  void processWebSocketRequest();
  void socketDisconnected();

};


#endif // SV_RESTAPI_SERVER_H
