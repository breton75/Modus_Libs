#include "sv_log_server.h"

//using namespace sv::log;


/** ********** SvWebServer ************ **/

httplog::SvHttpEventlog::SvHttpEventlog():
  modus::SvAbstractProvider(),
  m_server(new QTcpServer(this)),
  m_is_active(false)
{
  QDBusConnection::sessionBus().connect(QString(), QString("/%1").arg(P_INTERFACE), DBUS_SERVER_NAME, "message", this, SLOT(messageSlot(const QString&,int,const QString&,const QString&,const QString&)));
  QDBusConnection::sessionBus().connect(QString(), QString("/%1").arg(P_PROTOCOL),  DBUS_SERVER_NAME, "message", this, SLOT(messageSlot(const QString&,int,const QString&,const QString&,const QString&)));
  QDBusConnection::sessionBus().connect(QString(), QString("/%1").arg(P_SIGNAL),    DBUS_SERVER_NAME, "message", this, SLOT(messageSlot(const QString&,int,const QString&,const QString&,const QString&)));
  QDBusConnection::sessionBus().connect(QString(), QString("/%1").arg(P_STORAGE),   DBUS_SERVER_NAME, "message", this, SLOT(messageSlot(const QString&,int,const QString&,const QString&,const QString&)));
  QDBusConnection::sessionBus().connect(QString(), QString("/%1").arg(P_PROVIDER),  DBUS_SERVER_NAME, "message", this, SLOT(messageSlot(const QString&,int,const QString&,const QString&,const QString&)));
}

httplog::SvHttpEventlog::~SvHttpEventlog()
{

}

void httplog::SvHttpEventlog::messageSlot(const QString& entity, int id, const QString& type, const QString& time, const QString& message)
{
  for(QTcpSocket* client: m_clients) {

    client->write(QString("%1%2:%3:%4\n%5").arg(entity).arg(id).arg(time).arg(type).arg(message).toUtf8());

  }
}

bool httplog::SvHttpEventlog::configure(modus::ProviderConfig* config)
{
  p_config = config;

  try {

    m_params = httplog::Params::fromJsonString(p_config->params);

    if (!m_server->listen(QHostAddress::Any, m_params.port))
    {
      p_last_error = QString("Ошибка запуска сервера %1: %2").arg(p_config->name).arg(m_server->errorString());

      return false;

    };

    connect(m_server, &QTcpServer::newConnection, this, &httplog::SvHttpEventlog::newConnection);

    return true;

  }
  catch(SvException e)
  {
    p_last_error = e.error;

    return false;
  }
}

void httplog::SvHttpEventlog::stop()
{
  m_is_active = false;
  m_server->close();
  qDeleteAll(m_clients.begin(), m_clients.end());

}

void httplog::SvHttpEventlog::newConnection()
{
  QTcpSocket *client = m_server->nextPendingConnection();

  if(!client)
    return;

  connect(client, &QTcpSocket::readyRead, this, &httplog::SvHttpEventlog::processRequest);
  connect(client, &QTcpSocket::disconnected, this, &httplog::SvHttpEventlog::socketDisconnected);

  m_clients << client;

}

void httplog::SvHttpEventlog::socketDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

    if (client) {
        m_clients.removeAll(client);
        client->deleteLater();
    }
}

void httplog::SvHttpEventlog::processRequest()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

    QByteArray req = client->readAll();

    restapi::HttpRequest request = restapi::HttpRequest::parse(req);

    emit message(QString(req), sv::log::llDebug, sv::log::mtRequest);


    if((request.method == "GET"))
      client->write(reply_get());

//    else if (is_POST)
//      client->write(reply_POST(parts));

    client->flush(); // waitForBytesWritten(); //


  // нужно закрыть сокет
//  client->close();

}

//void httplog::SvHttpEventlog::run()
//{
//  while(true)
//  {
//    if(m_server->waitForNewConnection(100))
//    {
//      QTcpSocket *client = m_server->nextPendingConnection();

//      connect(client, &QTcpSocket::readyRead, this, &httplog::SvHttpEventlog::processRequest);
//      connect(client, &QTcpSocket::disconnected, this, &httplog::SvHttpEventlog::socketDisconnected);

//      m_clients << client;

//    }
//  }
//}

QByteArray httplog::SvHttpEventlog::reply_get()
{
  return QByteArray()
                      .append(QString("HTTP/1.1 200 OK\r\n\
                              Content-Type: text/html; charset=\"utf-8\"\r\n\r\n\
                              <html>\
                              <head><meta charset=\"UTF-8\"><title>Журнал событий</title>\n\
                                <script>\n\
                                  var get_log = function() {\
                                    const webSocket = new WebSocket('ws://' + location.origin);\n\
                                    \
                                    webSocket.onopen = event => {\n\
                                      alert('onopen');\n\
                                      webSocket.send(\"Hello Web Socket!\");\n\
                                    };\n\n\
                                    \
                                    webSocket.onmessage = event => {\
                                      alert('onmessage, ' + event.data);\
                                    };\
                                    \
                                    webSocket.onclose = event => {\n\
                                      alert('onclose');\n\
                                    };\n\
                                  }\
                                </script>\n\
                              <head>\
                              <body>\
                              <button id=\"bnGetLog\" onclick=\"get_log()\" >GET</button>\
                              <p style=\"text-align:center\"><textarea cols=\"150\" name=\"text\" rows=\"50\"></p>\
                              </body></html>\n")
                              .toUtf8());
//  <p><input name=\"bnGetLog\" type=\"button\" value=\"get\" /></p>

}

/** ********** EXPORT ************ **/
modus::SvAbstractProvider* create()
{
//  wd::SvAbstractServer* server = ;
  return new httplog::SvHttpEventlog();
}
