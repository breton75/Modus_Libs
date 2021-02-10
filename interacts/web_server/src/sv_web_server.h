#ifndef SV_WEB_SERVER_H
#define SV_WEB_SERVER_H

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

#include "webserver_global.h"

#include "../../../Modus/global/interact/sv_abstract_interact.h"
#include "../../../Modus/global/signal/sv_signal.h"

#include "params.h"


extern "C" {

    WEBSERVER_SHARED_EXPORT modus::SvAbstractInteract* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace websrv {

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

  class SvWebServer;

}

class websrv::SvWebServer: public modus::SvAbstractInteract
{
  Q_OBJECT

public:
  explicit SvWebServer();
  ~SvWebServer();

  bool init(modus::InteractConfig* config);

  void start();
  void stop();

  void addSignal(modus::SvSignal* signal) throw (SvException);
  void bindSignalList(QList<modus::SvSignal*>* signalList)  throw (SvException) Q_DECL_OVERRIDE;

  const QMap<int, modus::SvSignal*>*      signalsById()   const { return &m_signals_by_id;   }
  const QHash<QString, modus::SvSignal*>* signalsByName() const { return &m_signals_by_name; }

private:
//  websrv::SvWebTcpServer m_server;
  QTcpServer* m_web_server;
  QList<QTcpSocket*> m_clients;

  websrv::Params m_params;

  sv::SvAbstractLogger* m_logger = nullptr;

  QMap<int, modus::SvSignal*>      m_signals_by_id;//   = QMap<int, SvSignal*>();
  QHash<QString, modus::SvSignal*> m_signals_by_name;// = QHash<QString, SvSignal*>();

  QByteArray reply_GET(QList<QByteArray> &parts);
  QByteArray reply_POST(QList<QByteArray> &parts);

  void run() {}
//public slots:
//  void threadFinished();

private slots:
  void newConnection();
  void processRequest();
  void socketDisconnected();

//signals:
//  void stopThreads();

};

//class websrv::SvWebTcpServer: public QTcpServer
//{
//    Q_OBJECT

//public:
//    SvWebTcpServer(QObject *parent = 0);

//protected:
//    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE
//    {
//      websrv::SvWebServerThread *thread = new websrv::SvWebServerThread(socketDescriptor, this);
//      connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//      thread->start();
//    }

//};

//class websrv::SvWebServerThread: public QThread // asrv::SvAbstractServerThread
//{
//  Q_OBJECT

//public:
//  explicit SvWebServerThread(qintptr socket_descriptor, websrv::SvWebServer* web_server, websrv::Params* params, QObject *parent):
//    QThread(parent),
//    m_web_server(web_server),
//    m_socket_descriptor(socket_descriptor),
//    m_params(params)
//  {
//    m_logger = m_web_server->logger();
//  }

//  ~SvWebServerThread()
//  {  }


//  void run() Q_DECL_OVERRIDE;


//private:
////  asrv::SvAbstractServer* m_server;
//  websrv::SvWebServer* m_web_server;

//  qintptr m_socket_descriptor;
////  QTcpSocket m_client;

//  sv::SvAbstractLogger* m_logger = nullptr;

//  websrv::Params* m_params;

//  bool m_started;

//  QByteArray reply_GET(QList<QByteArray> &parts);
//  QByteArray reply_POST(QList<QByteArray> &parts);
////  void reply_GET_error(QTcpSocket& m_client, int errorCode, QString errorString);

//public slots:
//  void stop();

//};


#endif // SV_WEB_SERVER_H
