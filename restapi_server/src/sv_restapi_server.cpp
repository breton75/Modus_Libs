#include "sv_restapi_server.h"

//using namespace sv::log;


/** ********** SvWebServer ************ **/

restapi::SvRestAPI::SvRestAPI():
  modus::SvAbstractInteract(),
  m_web_server(new QTcpServer(this)),
  m_is_active(false)
{

}

restapi::SvRestAPI::~SvRestAPI()
{

}

bool restapi::SvRestAPI::init(modus::InteractConfig* config)
{
  p_config = config;

  try {

    m_params = restapi::Params::fromJsonString(p_config->params);

    if (!m_web_server->listen(QHostAddress::Any, m_params.port))
    {
      p_last_error = QString("Ошибка запуска сервера %1: %2").arg(p_config->name).arg(m_web_server->errorString());

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

void restapi::SvRestAPI::stop()
{
  m_is_active = false;
  m_web_server->close();
  qDeleteAll(m_clients.begin(), m_clients.end());

}

//void restapi::SvRestAPI::newConnection()
//{
//  QTcpSocket *client = m_web_server->nextPendingConnection();

//  connect(client, &QTcpSocket::readyRead, this, &restapi::SvRestAPI::processRequest);
//  connect(client, &QTcpSocket::disconnected, this, &restapi::SvRestAPI::socketDisconnected);

//  m_clients << client;

//}

void restapi::SvRestAPI::socketDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());

    if (client) {
        m_clients.removeAll(client);
        client->deleteLater();
    }
}

void restapi::SvRestAPI::processRequest()
{
    QTcpSocket *m_client = qobject_cast<QTcpSocket *>(sender());

    QByteArray request = m_client->readAll();

    QList<QByteArray> parts = request.split('\n');

    if((parts.count() < 2))
      return;

    bool is_GET  = parts.at(0).toUpper().startsWith("GET");
    bool is_POST = parts.at(0).toUpper().startsWith("POST");

    if(!(is_GET || is_POST))
      return;

    emit message(QString(request), sv::log::llDebug2, sv::log::mtDebug);

//    if(m_logger && m_logger->options().log_level >= sv::log::llDebug2)
//    {
//      QStringList sd = QString(request).split("\r\n");
//      for(QString d: sd)
//        *m_logger << sv::log::llDebug2 << sv::log::mtDebug << d << sv::log::endl;
//    }

    if(is_GET)
      m_client->write(reply_GET(parts));

    else if (is_POST)
      m_client->write(reply_POST(parts));

    m_client->flush(); // waitForBytesWritten(); //


  // нужно закрыть сокет
  m_client->close();

}

void restapi::SvRestAPI::run()
{
  while(true)
  {
    if(m_web_server->waitForNewConnection(100))
    {
      QTcpSocket *client = m_web_server->nextPendingConnection();

      connect(client, &QTcpSocket::readyRead, this, &restapi::SvRestAPI::processRequest);
      connect(client, &QTcpSocket::disconnected, this, &restapi::SvRestAPI::socketDisconnected);

      m_clients << client;

    }
  }
}

QByteArray restapi::SvRestAPI::reply_GET(QList<QByteArray> &parts)
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

  QString file = QString(parts.at(0).split(' ').at(1));

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

QByteArray restapi::SvRestAPI::reply_POST(QList<QByteArray> &parts)
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

  QStringList r1 = QString(parts.last()).split('?');

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

//void websrv::SvWebServerThread::reply_GET_error(QTcpSocket& m_client, int errorCode, QString errorString)
//{
//  if(m_logger)
//    *m_logger <<llError << mtError << errorString << sv::log::endl;

//  QTextStream replay(&m_client);
//  replay.setAutoDetectUnicode(true);

//  replay << "HTTP/1.1 %1 Error"
//         << "Content-Type: text/html; charset=\"utf-8\"\r\n\r\n"
//         << QString("<html>"
//                        "<head><meta charset=\"UTF-8\"><title>Ошибка</title><head>"
//                        "<body>"
//                        "<p style=\"font-size: 16\">%2</p>"
//                        "<a href=\"index.html\" style=\"font-size: 14\">На главную</a>"
//                        "<p>%3</p>"
//                        "</body></html>\n")
//            .arg(errorCode)
//            .arg(errorString)
//            .arg(QDateTime::currentDateTime().toString());
//}

//void websrv::SvWebServerThread::stop()
//{
//  m_started = false;
//}


/** ********** EXPORT ************ **/
modus::SvAbstractInteract* create()
{
//  wd::SvAbstractServer* server = ;
  return new restapi::SvRestAPI();
}
