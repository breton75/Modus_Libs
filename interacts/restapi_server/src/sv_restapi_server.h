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

#include "restapi_server_global.h"

#include "../../../../Modus/global/interact/sv_abstract_interact.h"
#include "../../../../Modus/global/global_defs.h"
#include "params.h"

extern "C" {

    INTERACT_SHARED_EXPORT modus::SvAbstractInteract* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace restapi {

  typedef QMap<QString, QString> HttpFields;

  struct HttpFld {

    const QList<QString> fields = QList<QString>() << "Date"
                                                   << "Content-Type"
                                                   << "Content-Length"
                                                   << "Last-Modified"
                                                   << "Connection"
                                                   << "Upgrade"
                                                   << "Origin" << "Host" << "Sec-WebSocket-Key"
                                                   << "Sec-WebSocket-Version" << "Sec-WebSocket-Accept"
                                                   << "Sec-WebSocket-Protocol";

  };

  struct HttpRequest {

    HttpRequest() {

      qFill() fields.insertMulti()
    }

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

  bool configure(modus::InteractConfig* config) override;

  void start() override;
  void stop() override;

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

  QByteArray reply_http_get(HttpRequest &request);
  QByteArray reply_http_post(HttpRequest &request);
  QByteArray reply_ws_get(HttpRequest &request);

//  void processRequests() override;

private slots:
  void newConnection();
  void processHttpRequest();
  void processWebSocketRequest();
  void socketDisconnected();

};


#endif // SV_RESTAPI_SERVER_H
