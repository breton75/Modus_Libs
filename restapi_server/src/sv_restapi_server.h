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

#include "../../../Modus/global/interact/sv_abstract_interact.h"
#include "../../../Modus/global/global_defs.h"
#include "params.h"

extern "C" {

    INTERACT_SHARED_EXPORT modus::SvAbstractInteract* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace restapi {

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

  bool init(modus::InteractConfig* config) override;

  void stop() override;

  const QMap<int, modus::SvSignal*>*      signalsById()   const { return &m_signals_by_id;   }
  const QHash<QString, modus::SvSignal*>* signalsByName() const { return &m_signals_by_name; }

private:
  QTcpServer* m_web_server;
  QList<QTcpSocket*> m_clients;

  restapi::Params m_params;

  QMap<int, modus::SvSignal*>      m_signals_by_id;
  QHash<QString, modus::SvSignal*> m_signals_by_name;

  bool m_is_active;

  QByteArray reply_GET(QList<QByteArray> &parts);
  QByteArray reply_POST(QList<QByteArray> &parts);

  void run() override;

private slots:
  void processRequest();
  void socketDisconnected();

};


#endif // SV_RESTAPI_SERVER_H
