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
#include "../../../../Modus/global/restapi/http_global.h"
#include "../../../../Modus/global/restapi/entity_task_option.h"

#include "restapi_server_defs.h"

#define M_SIGNAL_ID_NOT_FOUND       "{\"value\":\"get Сигнал с id %1 в конфигурации не найден\"},"
#define M_SIGNAL_NAME_NOT_FOUND     "{\"value\":\"get Сигнал '%1' в конфигурации не найден\"},"
#define M_SIGNAL_NOT_SUITABLE_TYPE  "{\"value\":\"Нельзя изменить значение сигнала с id %1. " \
                                    "Могут быть изменены только сигналы с вариантом использования OUT и VAR\"},"

extern "C" {

    INTERACT_SHARED_EXPORT modus::SvAbstractProvider* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace restapi {

  class SvRestAPI;

}

class restapi::SvRestAPI: public modus::SvAbstractProvider
{
  Q_OBJECT

public:
  explicit SvRestAPI();
  ~SvRestAPI();

  bool configure(modus::ProviderConfig* config, modus::Configuration* configuration) override;

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

  QByteArray reply_http_get(const http::HttpRequest &request);
  QByteArray reply_http_get_params(const http::HttpRequest &request);
  QByteArray reply_http_post(const http::HttpRequest &request);
  QByteArray reply_ws_get(const http::HttpRequest &request);

  QByteArray getEntityData(const QString& entity, const QString& task, const QString& option, const QString& data, const char separator = ',');

  QByteArray getSignalsData(const QString& task, const QString& option, const QString& data, const char separator = ',');
  QByteArray getSignalsValues(const QString& option, const QString& data, const char separator = ',');

  QByteArray getConfigurationData(const QString& task, const QString& option, const QString& data, const char separator = ',');

  QByteArray getHttpError(int errorCode, QString errorString);

  QByteArray setSignalValues(const QJsonObject& jo);
//  QByteArray setEventlog(const QJsonObject& jo);

private slots:
  void newConnection();
  void processHttpRequest();
  void processWebSocketRequest();
  void socketDisconnected();

};


#endif // SV_RESTAPI_SERVER_H
