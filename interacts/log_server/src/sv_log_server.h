#ifndef SV_LOG_SERVER_H
#define SV_LOG_SERVER_H

//#include "QtWebSockets/QTcpSocketserver.h"
//#include "QtWebSockets/QTcpSocket.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDBusConnection>

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

#include "log_server_global.h"

#include "../../../../Modus/global/interact/sv_abstract_interact.h"
#include "../../../../Modus/global/global_defs.h"
#include "../../../../Modus/global/restapi/http_global.h"
#include "../../../../Modus/global/dbus/sv_dbus.h"
#include "params.h"

extern "C" {

    INTERACT_SHARED_EXPORT modus::SvAbstractProvider* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace httplog {

  class SvHttpEventlog: public modus::SvAbstractProvider
  {
    Q_OBJECT

  public:
    explicit SvHttpEventlog();
    ~SvHttpEventlog();

    bool configure(modus::ProviderConfig* config) override;

    void start() override { }

  //  const QMap<int, modus::SvSignal*>*      signalsById()   const { return &m_signals_by_id;   }
  //  const QHash<QString, modus::SvSignal*>* signalsByName() const { return &m_signals_by_name; }

  private:
    QTcpServer* m_server;
    QList<QTcpSocket*> m_clients;

    httplog::Params m_params;

  //  QMap<int, modus::SvSignal*>      m_signals_by_id;
  //  QHash<QString, modus::SvSignal*> m_signals_by_name;

    bool m_is_active;

    QByteArray reply_get();
//    QByteArray reply_POST(QList<QByteArray> &parts);

  //  void run() override;

  public slots:
    void stop() override;

  private slots:
    void newConnection();
    void processRequest();
    void socketDisconnected();

    void messageSlot(const QString& entity, int id, const QString& type, const QString& time, const QString& message);

  };

}

#endif // SV_RESTAPI_SERVER_H
