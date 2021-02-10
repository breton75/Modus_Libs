﻿#ifndef SV_WEB_SERVER_H
#define SV_WEB_SERVER_H

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

#include "webserver_global.h"

#include "../../../global/sv_abstract_server.h"
#include "../../../global/sv_signal.h"

#include "params.h"


extern "C" {

    WEBSERVERSHARED_EXPORT wd::SvAbstractServer* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace wd {

//  class SvWebServer;
  class SvWebServerThread;

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

  class SvWebServer: public wd::SvAbstractServer
  {
    Q_OBJECT

  public:
    explicit SvWebServer(sv::SvAbstractLogger* logger = nullptr, QObject *parent = 0);
    ~SvWebServer();

    bool configure(const wd::ServerConfig& config);

    bool init();

    void start();
    void stop();

    void addSignal(SvSignal* signal)  throw (SvException) Q_DECL_OVERRIDE;

    const QMap<int, SvSignal*>*      signalsById()   const { return &m_signals_by_id;   }
    const QHash<QString, SvSignal*>* signalsByName() const { return &m_signals_by_name; }

  private:
    QTcpServer m_server;
    QList<SvWebServerThread*> m_clients;

    Params m_params;

    sv::SvAbstractLogger* m_logger;

    QMap<int, SvSignal*>      m_signals_by_id;//   = QMap<int, SvSignal*>();
    QHash<QString, SvSignal*> m_signals_by_name;// = QHash<QString, SvSignal*>();

  public slots:
    void threadFinished();

  private slots:
    void newConnection();

  signals:
    void stopThreads();

  };

  class SvWebServerThread: public QThread // asrv::SvAbstractServerThread
  {
    Q_OBJECT

  public:
    explicit SvWebServerThread(qintptr socket_descriptor, SvWebServer* server, Params* params):
      m_server(server),
      m_socket_descriptor(socket_descriptor),
      m_params(params)
    {

    }

    ~SvWebServerThread()
    {  }


    void run() Q_DECL_OVERRIDE;


  private:
  //  asrv::SvAbstractServer* m_server;
    SvWebServer* m_server;

    qintptr m_socket_descriptor;
    QTcpSocket m_client;

    sv::SvAbstractLogger* m_logger;

    Params* m_params;

    bool m_started;

    void reply_GET(QList<QByteArray> &parts);
    void reply_POST(QList<QByteArray> &parts);
    void reply_GET_error(int errorCode, QString errorString);

  public slots:
    void stop();

  };
}

#endif // SV_WEB_SERVER_H
