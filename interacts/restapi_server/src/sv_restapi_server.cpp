#include "sv_restapi_server.h"

//using namespace sv::log;


/** ********** SvWebServer ************ **/

restapi::SvRestAPI::SvRestAPI():
  modus::SvAbstractInteract(),
  m_server(new QTcpServer()),
  m_is_active(false)
{

}

restapi::SvRestAPI::~SvRestAPI()
{

}

bool restapi::SvRestAPI::configure(modus::InteractConfig* config)
{
  p_config = config;

  try {

    m_params = restapi::Params::fromJsonString(p_config->params);

//    m_web_server->moveToThread(this);

    if (!m_server->listen(QHostAddress::Any, m_params.port))
    {
      p_last_error = QString("Ошибка запуска сервера %1: %2").arg(p_config->name).arg(m_server->errorString());

      return false;

    };

//    connect(m_web_server, &QTcpServer::newConnection, this, &restapi::SvRestAPI::newConnection);

    return true;

  }
  catch(SvException e)
  {
    p_last_error = e.error;

    return false;
  }
}

void restapi::SvRestAPI::start()
{
  connect(m_server, &QTcpServer::newConnection, this, &restapi::SvRestAPI::newConnection);
}

void restapi::SvRestAPI::stop()
{
  disconnect(m_server, &QTcpServer::newConnection, this, &restapi::SvRestAPI::newConnection);
  m_is_active = false;

  foreach (QTcpSocket* client, m_websocket_clients)
    client->close();

  qDeleteAll(m_websocket_clients.begin(), m_websocket_clients.end());

  m_server->close();
  delete m_server;

}

void restapi::SvRestAPI::newConnection()
{
  QTcpSocket *client = m_server->nextPendingConnection();

  connect(client, &QTcpSocket::readyRead, this, &restapi::SvRestAPI::processHttpRequest);
  connect(client, &QTcpSocket::disconnected, this, &restapi::SvRestAPI::socketDisconnected);

}

void restapi::SvRestAPI::socketDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

    if (client)
        client->deleteLater();

}

void restapi::SvRestAPI::processHttpRequest()
{
  QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

//    QTextStream serialized(client);
//    serialized.readAll();
qDebug() << 1;
  QList<QByteArray> rawreq = client->readAll().split('\n');
  if(!rawreq.count())
    return;

  QList<QByteArray> rawheader = rawreq.at(0).split(' ');
  if(rawheader.count() < 3)
    return;

  for(QByteArray line: rawreq)
    emit message(QString(line), sv::log::llDebug2, sv::log::mtDebug);

  HttpRequest request;

  // разбираем заголовок запроса
  request.method   = QString(rawheader.at(0).trimmed());
  request.resourse = QString(rawheader.at(1).split('?').count() > 0 ? rawheader.at(1).split('?').at(0).trimmed() : rawheader.at(1));
  request.params   = QString(rawheader.at(1).split('?').count() > 1 ? rawheader.at(1).split('?').at(1).trimmed() : "");
  request.protocol = QString(rawheader.at(2).split('/').count() > 0 ? rawheader.at(2).split('/').at(0).trimmed() : rawheader.at(2));
  request.version  = QString(rawheader.at(2).split('/').count() > 1 ? rawheader.at(2).split('/').at(1).trimmed() : "");
qDebug() << 2;
  // разбираем поля запроса
  int i = 1;
  while(i < rawreq.count() && !rawreq.at(i).trimmed().isEmpty())
  {
    QList<QByteArray> fld = rawreq.at(i).split(':');
    attr
    if(attr.count() != 2)
      return;

    request.attributes.insert(QString(attr.at(0).trimmed()), QString(attr.at(1).trimmed()));

    i++;
  }
qDebug() << 3;
  // выбираем данные запроса
  i++;
  if(i < rawreq.count() && !rawreq.at(i).trimmed().isEmpty())
  {
    request.data = rawreq.at(i).trimmed();
  }

qDebug() << (request.attributes.contains("Upgrade") ? request.attributes.value("Upgrade") : "no upgrade");
  // если клиент запрашивает изменение протокола на WebSocket, то отвечаем на запрос http, меняем обработчик и НЕ закрываем сокет
  if(request.attributes.contains("Upgrade") && request.attributes.value("Upgrade") == "websocket")
  {
    if(request.method == "GET")
      client->write(reply_ws_get(request));

    disconnect(client, &QTcpSocket::readyRead, this, &restapi::SvRestAPI::processHttpRequest);
    connect(client, &QTcpSocket::readyRead, this, &restapi::SvRestAPI::processWebSocketRequest);

    m_websocket_clients << client;

  }
  else {

    if(request.method == "GET")
      client->write(reply_http_get(request));

    else if (request.method == "POST")
      client->write(reply_http_post(request));

    client->flush(); // waitForBytesWritten(); //

    // нужно закрыть сокет
    client->close();

  }
}

void restapi::SvRestAPI::processWebSocketRequest()
{
  QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

  QList<QByteArray> rawreq = client->readAll().split('\n');
  if(!rawreq.count())
    return;

  QList<QByteArray> rawheader = rawreq.at(0).split(' ');
  if(rawheader.count() < 3)
    return;

  for(QByteArray line: rawreq)
    emit message(QString(line), sv::log::llDebug2, sv::log::mtDebug);

  HttpRequest request;

  // разбираем заголовок запроса
  request.method   = QString(rawheader.at(0).trimmed());
  request.resourse = QString(rawheader.at(1).split('?').count() > 0 ? rawheader.at(1).split('?').at(0).trimmed() : rawheader.at(1));
  request.params   = QString(rawheader.at(1).split('?').count() > 1 ? rawheader.at(1).split('?').at(1).trimmed() : "");
  request.protocol = QString(rawheader.at(2).split('/').count() > 0 ? rawheader.at(2).split('/').at(0).trimmed() : rawheader.at(2));
  request.version  = QString(rawheader.at(2).split('/').count() > 1 ? rawheader.at(2).split('/').at(1).trimmed() : "");
}

QByteArray restapi::SvRestAPI::reply_http_get(HttpRequest &request)
{
  auto getErr = [=](int errorCode, QString errorString) -> QByteArray {

    emit message(errorString, sv::log::llError, sv::log::mtError);

//      if(m_logger)
//        *m_logger <<llError << mtError << errorString << sv::log::endl;

      return QByteArray()
                        .append(QString("HTTP/1.1 %1 Error" \
                                "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
                                "<html>"
                                "<head><meta charset=\"UTF-8\"><title>Ошибка</title><head>"
                                "<body>"
                                "<p style=\"font-size: 16\">%2</p>"
                                "<a href=\"index.html\" style=\"font-size: 14\">На главную</a>"
                                "<p>%3</p>"
                                "</body></html>\n")
                                    .arg(errorCode)
                                    .arg(errorString)
                                    .arg(QDateTime::currentDateTime().toString())
                                .toUtf8());
  };

  QDir dir(m_params.html_path);

  QString file = QString(request.resourse);

  if(file.startsWith('/'))
    file.remove(0, 1);

  if(QFileInfo(dir, file).isDir())
    file = m_params.index_file;


  QByteArray replay = QByteArray();

  QFile f(dir.absoluteFilePath(file));

  if(!f.exists())
    replay = getErr(404, QString("Файл отсутствует: %1").arg(file));

  else if(!f.open(QIODevice::ReadOnly))
    replay = getErr(500, f.errorString());

  else
  {
    QString content_type = ContentTypeBySuffix.contains(QFileInfo(file).suffix())
                                   ? ContentTypeBySuffix.value(QFileInfo(file).suffix())
                                   : "application/octet-stream"; //двоичный файл без указания формата (RFC 2046)


    replay.append("HTTP/1.1 200 Ok\r\n")
          .append(QString("Content-Type: %1; charset=\"utf-8\"\r\n\r\n").arg(content_type).toUtf8())
          .append(f.readAll())
          .append("\r\n");

  }

  if(f.isOpen())
    f.close();

  return replay;

}

QByteArray restapi::SvRestAPI::reply_http_post(HttpRequest &request)
{
  auto Var2Str = [](QVariant value) -> QString {

      if(value.isValid())
      {
        switch (value.type()) {
          case QVariant::Int:

            return QString::number(value.toInt());
            break;

          case QVariant::Double:

            return QString::number(value.toDouble());
            break;

          default:
            return "";

        }
      }

      return "";

  };

  QStringList r1 = QString(request.data).split('?');

  if(r1.count() < 2)
    return QByteArray();

  QString json = ""; // формируем ответ в формате JSON

  if(r1.at(0) == "names")
  {

    QStringList names = QString(r1.at(1)).split(',');

    for(QString name: names)
    {
      if(name.trimmed().isEmpty())
        continue;

      if(signalsByName()->contains(name))
        json.append(QString("{\"name\":\"%1\",\"value\":\"%2\"},")
                      .arg(name).arg(Var2Str(signalsByName()->value(name)->value())));

    }


    if(!json.isEmpty()) json.chop(1);


  }

  else if(r1.at(0) == "ids")
  {
    QStringList ids = QString(r1.at(1)).split(',');

    for(QString curid: ids)
    {
      if(curid.trimmed().isEmpty())
        continue;

      bool ok;
      int id = curid.toInt(&ok);

      if(ok && signalsById()->contains(id))
        json.append(QString("{\"id\":\"%1\",\"value\":\"%2\"},")
                      .arg(id).arg(Var2Str(signalsById()->value(id)->value())));

    }

    if(!json.isEmpty()) json.chop(1);

  }

  QByteArray http = QByteArray()
                    .append("HTTP/1.0 200 Ok\r\n")
                    .append("Content-Type: text/json; charset=\"utf-8\"\r\n")
                    .append(QString("Content-Length: %1\r\n").arg(json.length() + 2))
                    .append("Access-Control-Allow-Origin: *\r\n")
                    .append("Access-Control-Allow-Headers: *\r\n")
                    .append("Origin: file://\r\n\r\n")        //! обязательно два!
                    .append("[").append(json).append("]\r\n");

//  if(m_logger && m_logger->options().log_level >= sv::log::llDebug2)
//    *m_logger << sv::log::llDebug2 << sv::log::mtDebug << QString(http) << sv::log::endl;

  emit message(QString(http), sv::log::llDebug2, sv::log::mtDebug);

  return http;

}

QByteArray restapi::SvRestAPI::reply_ws_get(HttpRequest &request)
{
  auto getErr = [=](int errorCode, QString errorString) -> QByteArray {

    emit message(errorString, sv::log::llError, sv::log::mtError);

//      if(m_logger)
//        *m_logger <<llError << mtError << errorString << sv::log::endl;

      return QByteArray()
                        .append(QString("HTTP/1.1 %1 Error" \
                                "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
                                "<html>"
                                "<head><meta charset=\"UTF-8\"><title>Ошибка</title><head>"
                                "<body>"
                                "<p style=\"font-size: 16\">%2</p>"
                                "<a href=\"index.html\" style=\"font-size: 14\">На главную</a>"
                                "<p>%3</p>"
                                "</body></html>\n")
                                    .arg(errorCode)
                                    .arg(errorString)
                                    .arg(QDateTime::currentDateTime().toString())
                                .toUtf8());
  };

  QByteArray replay = QByteArray();

  replay.append(QString("%1/%2 101 Switching Protocols\r\n").arg(QString(request.protocol)).arg(QString(request.version)));
  replay.append(QString("Date: Wed, %1\r\f").arg(QDateTime::currentDateTimeUtc().toString("dd MMM yyyy hh:mm:ss t")));
  replay.append(QString("Connection: Upgrade\r\n"));
  replay.append(QString("Upgrade: WebSocket\r\f"));

qDebug() << QString(replay);
  return replay;

}


/** ********** EXPORT ************ **/
modus::SvAbstractInteract* create()
{
  modus::SvAbstractInteract* server = new restapi::SvRestAPI();
  return server;
}
