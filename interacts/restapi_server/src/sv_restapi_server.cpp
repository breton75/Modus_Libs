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

bool restapi::SvRestAPI::configure(modus::InteractConfig* config, modus::Configuration* configuration)
{
  p_config = config;
  p_modus_configiration = configuration;

  try {

    m_params = restapi::Params::fromJsonString(p_config->params);

//    m_web_server->moveToThread(this);

    if (!m_server->listen(QHostAddress::Any, m_params.port))
    {
      p_last_error = QString("Ошибка запуска сервера %1: %2").arg(p_config->name).arg(m_server->errorString());

      return false;

    };

    return true;

  }
  catch(SvException e)
  {
    p_last_error = e.error;

    return false;
  }
}

bool restapi::SvRestAPI::bindSignal(modus::SvSignal* signal)
{
  if(!modus::SvAbstractInteract::bindSignal(signal))
    return false;

  if(!m_signals_by_id.contains(signal->config()->id)) m_signals_by_id.insert(signal->config()->id, signal);
  if(!m_signals_by_name.contains(signal->config()->name)) m_signals_by_name.insert(signal->config()->name, signal);

  return true;

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

  QByteArray req = client->readAll();
  emit message(QString(req), sv::log::llDebug, sv::log::mtRequest);


  QList<QByteArray> rawreq = req.split('\n');
  if(!rawreq.count())
    return;

  QList<QByteArray> rawheader = rawreq.at(0).split(' ');
  if(rawheader.count() < 3)
    return;


//  for(QByteArray line: rawreq)
//    qDebug() << line;
//    emit message(QString(line.trimmed()), sv::log::llDebug2, sv::log::mtDebug);

  HttpRequest request;

  // разбираем заголовок запроса
  request.method   = QString(rawheader.at(0).trimmed());
  request.resourse = QString(rawheader.at(1).split('?').count() > 0 ? rawheader.at(1).split('?').at(0).trimmed() : rawheader.at(1));
  request.params   = QString(rawheader.at(1).split('?').count() > 1 ? rawheader.at(1).split('?').at(1).trimmed() : "");
  request.protocol = QString(rawheader.at(2).split('/').count() > 0 ? rawheader.at(2).split('/').at(0).trimmed() : rawheader.at(2));
  request.version  = QString(rawheader.at(2).split('/').count() > 1 ? rawheader.at(2).split('/').at(1).trimmed() : "");

  // разбираем поля запроса
  int i = 1;
  while(i < rawreq.count() && !rawreq.at(i).trimmed().isEmpty())
  {
    QString r = QString(rawreq.at(i));

    int p = r.indexOf(QChar(':'));

    if(p >= 0) {

      QString field = r.left(p).toLower().trimmed();
      QString token = r.right(r.length() - p - 1).trimmed();

      if(Http_Field_List.contains(field))
        request.fields.insert(field, token);

    }

    i++;
  }

  // выбираем данные запроса
  i++;
  if(i < rawreq.count() && !rawreq.at(i).trimmed().isEmpty())
  {
    request.data = rawreq.at(i).trimmed();
  }

  // если клиент запрашивает изменение протокола на WebSocket, то отвечаем на запрос http, меняем обработчик и НЕ закрываем сокет
  if(request.fields.contains("upgrade") && request.fields.value("upgrade") == "websocket")
  {
    if(request.method == "GET")
      client->write(reply_ws_get(request));

    disconnect(client, &QTcpSocket::readyRead, this, &restapi::SvRestAPI::processHttpRequest);
    connect(client, &QTcpSocket::readyRead, this, &restapi::SvRestAPI::processWebSocketRequest);

    m_websocket_clients << client;

    QByteArray b;
    b.append(char(0x81));
    b.append(char(0x05));
//    b.append(char(0x00));
//    b.append(char(0x00));
    b.append("hello");

    client->write(b);


  }
  else {

    if(request.method == "GET") {

      if(request.params.isEmpty())
        client->write(reply_http_get(request));

      else
        client->write(reply_http_get_params(request));

    }

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

QByteArray restapi::SvRestAPI::reply_http_get(const HttpRequest &request)
{
  QDir dir(m_params.html_path);

  QString file = QString(request.resourse);

  if(file.startsWith('/'))
    file.remove(0, 1);

  if(QFileInfo(dir, file).isDir())
    file = m_params.index_file;


  QByteArray replay = QByteArray();

  QFile f(dir.absoluteFilePath(file));

  if(!f.exists())
    replay = getHttpError(404, QString("Файл отсутствует: %1").arg(file));

  else if(!f.open(QIODevice::ReadOnly))
    replay = getHttpError(500, f.errorString());

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

QByteArray restapi::SvRestAPI::reply_http_get_params(const HttpRequest &request)
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

  QStringList get_params = QString(request.params).split('&', QString::SplitBehavior::SkipEmptyParts);

  if(get_params.count() == 0)
    return getErr(404, QString("Неверный запрос"));

  QString entity = "";
  QString what = "";
  QString option = "";
  QString list   = "";

  for(QString param: get_params) {

    if(param.indexOf('=') < 1)
      return getErr(404, QString("Неверный запрос. Неопределенный параметр %1").arg(param));

    QString field = param.left(param.indexOf('='));
    QString tag   = param.right(param.length() - param.indexOf('=') - 1);

    if(field.toLower().trimmed() == "entity")
      entity = tag;

    else if(field.toLower().trimmed() == "what")
      what = tag;

    else if(field.toLower().trimmed() == "option")
      option = tag;

    else if(field.toLower().trimmed() == "list")
      list = tag;

  }

  QByteArray json = getEntityData(entity, what, option, list);

/*  QString entity_param = QString(get_params.at(0));


  if(!RestGetFieldsMap.contains(tag))
    return getErr(404, QString("Неизвестный параметр запроса '%1'").arg(tag));

  if(RestGetFieldsMap.value(tag) != RestGetFields::entity)
    return getErr(404, QString("Неверный запрос. Ожидалось поле 'entity'. Получено '%1'").arg(tag));

  if(!RestGetEntitiesMap.contains(entity))
    return getErr(404, QString("Неверный запрос. Неизвестная сущность '%1'").arg(entity));

  get_params.pop_front();
  QString json = ""; // формируем ответ в формате JSON

  switch (RestGetEntitiesMap.value(entity)) {

    case RestGetEntities::signal:

      json = processEntitySignal(get_params);

      break;

    case RestGetEntities::device:

      break;

    case RestGetEntities::storage:

      break;

    case RestGetEntities::analize:

      break;

    case RestGetEntities::configuration:

      break;

  }


  for(QString query: get_params) {

    if(query.indexOf('=') < 1)
      continue;

    QString query_field = query.left(query.indexOf('='));
    QString query_data  = query.right(query.length() - query.indexOf('=') - 1);

    if(!RestGetEntitiesMap.contains(query_field))
      continue;

    switch (RestGetEntitiesMap.value(query_field)) {
    case RestGetEntities::signal:

      json = handle_signal_request();

      break;
    default:
      break;
    }

    if(query_field == "names")
    {

      QStringList names = query_data.split(',');

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

    else if(query_field == "ids")
    {
      QStringList ids = query_data.split(',');

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
  }
  */

  QByteArray http = QByteArray()
                    .append("HTTP/1.0 200 Ok\r\n")
                    .append("Content-Type: text/json; charset=\"utf-8\"\r\n")
                    .append(QString("Content-Length: %1\r\n").arg(json.length() + 2))
                    .append("Access-Control-Allow-Origin: *\r\n")
                    .append("Access-Control-Allow-Headers: *\r\n")
                    .append("Origin: file://\r\n\r\n")        //! обязательно два!
                    .append(json).append("\r\n");

//  if(m_logger && m_logger->options().log_level >= sv::log::llDebug2)
//    *m_logger << sv::log::llDebug2 << sv::log::mtDebug << QString(http) << sv::log::endl;

  emit message(QString(http), sv::log::llDebug2, sv::log::mtDebug);

  return http;

}

QByteArray restapi::SvRestAPI::getEntityData(const QString& entity, const QString& what, const QString& option, const QString& list, const char separator)
{
  QByteArray result;

  switch (restapi::ewo::EntitiesTable.value(entity, restapi::ewo::noentity)) {

    case restapi::ewo::signal:

      result = getSignalsData(what, option, list, separator);

      break;

    case restapi::ewo::configuration:

      result = getConfigurationData(what, option, list, separator);

      break;

    default:
      result = "Не доступно в данной версии";
      break;

  }

  return result;

}

QByteArray restapi::SvRestAPI::getSignalsData(const QString& what, const QString& option, const QString& list, const char separator)
{
  QByteArray result;

  switch (restapi::ewo::WhatsTable.value(what, restapi::ewo::nowhat)) {

    case restapi::ewo::value:

      result = getSignalsValues(option, list, separator);

      break;

    case restapi::ewo::json:

      result = "Не доступно в данной версии";
      break;

    default:
      result = "Не доступно в данной версии";
      break;

  }

  return result;

}

QByteArray restapi::SvRestAPI::getSignalsValues(const QString& option, const QString& list, const char separator = ',')
{
  auto var2str = [](QVariant value) -> QString {

    QString result = "\"null\"";

    if(value.isValid() && !value.isNull())
    {
      switch (value.type()) {
/* Although this function is declared as returning QVariant::Type,
 * the return value should be interpreted as QMetaType::Type */
        case QMetaType::Int:
          result = QString::number(value.toInt());
          break;

        case QMetaType::UInt:
          result = QString::number(value.toUInt());
          break;

        case QMetaType::LongLong:
          result = QString::number(value.toLongLong());
          break;

        case QMetaType::ULongLong:
          result = QString::number(value.toULongLong());
          break;

        case QMetaType::Float:
          result = QString::number(value.toFloat());
          break;

        case QMetaType::Double:
          result = QString::number(value.toDouble());
          break;

        default:
          result = QString("\"Неизвестный тип сигнала: %1\"").arg(value.typeName());

      }
    }

    return result;

  };

  QByteArray values;
  QByteArray errors;

  values.append('{')
        .append("\"values\":[");
  errors.append("\"errors\":[");

  switch (restapi::ewo::OptionsTable.value(option, restapi::ewo::nooption)) {

    case restapi::ewo::Options::byid:
    {
      QList<QString> ids = list.split(QChar(separator), QString::SkipEmptyParts);

      if(ids.isEmpty())
        errors.append("{\"value\": \"Неверный запрос значений сигналов. Список идентификаторов (list) пуст.\"},");

      bool ok;
      for(QString id: ids) {

        int iid = id.toInt(&ok);

        if(ok) {

          modus::SvSignal* signal = m_signals_by_id.value(iid);

          if(signal)
            values.append(QString("{\"id\":%1,\"value\":%2},").arg(id).arg(var2str(signal->value())));

          else
            errors.append(QString("{\"value\":\"Сигнал с id '%1' не найден\"},").arg(id));

        }
        else
          errors.append(QString("{\"value\":\"Неверный id сигнала: '%1'\"},").arg(id));

      }

      break;
    }

    case ewo::Options::byname:
    {

      QList<QString> names = list.split(QChar(separator), QString::SkipEmptyParts);

      if(names.isEmpty())
        errors.append("{\"value\": \"Неверный запрос значений сигналов. Список имен сигналов (list) пуст.\"}");

      for(QString name: names) {

        modus::SvSignal* signal = m_signals_by_name.value(name);

        if(signal)
          values.append(QString("{\"name\":%1,\"value\":%2},").arg(name).arg(var2str(signal->value())));

        else
          errors.append(QString("{\"value\":\"Сигнал с именем '%1' не найден\"},").arg(name));


      }

      break;
    }

    default:
      errors.append(QString("{\"value\":\"Неверный запрос. Неизвестная опция '%1'\"},").arg(option));
      break;
    }

  if(values.endsWith(',')) values.chop(1);
  if(errors.endsWith(',')) errors.chop(1);

  errors.append(']');
  values.append(']').append(',').append(errors).append('}');

  return values;

}

QByteArray restapi::SvRestAPI::getConfigurationData(const QString& what, const QString& option, const QString& list, const char separator)
{
  return QByteArray(QString("Not avalable yet. %1 %2 %3 %4").arg(what,option,list).arg(separator).toUtf8());
}

QByteArray restapi::SvRestAPI::reply_http_post(const HttpRequest &request)
{
//  message(QString(request.data));

  QByteArray json   = QByteArray();
  json.append('{');

  QByteArray errors = QByteArray();

  QJsonParseError per;
  QJsonDocument jd = QJsonDocument::fromJson(request.data, &per); // запрос должен быть в формате JSON

  if(per.error != QJsonParseError::NoError) {

    return getHttpError(400, QString("Некорректный запрос на обновление данных.\n%1\n%2")
                        .arg(QString(request.data)).arg(per.errorString()));
  }

  QJsonObject jo = jd.object();
  if(jo.contains(P_SIGNALS) && jo.value(P_SIGNALS).isArray()) {

    QJsonArray ja = jo.value(P_SIGNALS).toArray();

    for(QJsonValue jv: ja) {

      if(!jv.isObject())
        continue;

      QJsonObject o = jv.toObject();

      if(!o.contains(P_VALUE))
        continue;

      if(o.contains(P_ID)) {

        int id = o.value(P_ID).toInt(-1);

        if(!m_signals_by_id.contains(id)) {

          errors.append(QString(M_SIGNAL_ID_NOT_FOUND).arg(id));

          emit message(QString(M_SIGNAL_ID_NOT_FOUND).arg(id), sv::log::llDebug, sv::log::mtError);

          continue;
        }

        if(m_signals_by_id.value(id)->config()->usecase != modus::OUT &&
           m_signals_by_id.value(id)->config()->usecase != modus::VAR) {

          errors.append(QString(M_SIGNAL_NOT_SUITABLE_TYPE).arg(id));
          emit message(QString(M_SIGNAL_NOT_SUITABLE_TYPE).arg(id), sv::log::llDebug, sv::log::mtError);

          continue;
        }

        emit message(jv.toString(), sv::log::llDebug, sv::log::mtParse);

        m_signals_by_id.value(id)->setValue(o.value(P_VALUE).toVariant());
//        qDebug() << m_signals_by_id.value(id)->value().toInt() << o.value("value").toVariant();

      }
      else if(o.contains(P_NAME)) {

          QString name = o.value(P_NAME).toString("");

          if(!m_signals_by_name.contains(name)) {

            errors.append(QString(M_SIGNAL_NAME_NOT_FOUND).arg(name));

            emit message(QString(M_SIGNAL_NAME_NOT_FOUND).arg(name), sv::log::llDebug, sv::log::mtError);

            continue;
          }

          if(m_signals_by_name.value(name)->config()->usecase != modus::UseCase::OUT ||
             m_signals_by_name.value(name)->config()->usecase != modus::UseCase::VAR) {

            errors.append(QString(M_SIGNAL_NOT_SUITABLE_TYPE).arg(name));
            emit message(QString(M_SIGNAL_NOT_SUITABLE_TYPE).arg(name), sv::log::llDebug, sv::log::mtError);

            continue;
          }

          emit message(jv.toString(), sv::log::llDebug, sv::log::mtParse);

          m_signals_by_name.value(name)->setValue(o.value(P_VALUE).toVariant());

        }
    }
  }

  if(!errors.isEmpty()) {

    if(errors.endsWith(',')) errors.chop(1);

    json.append(QString("\"errors\":[").toUtf8()).append(errors).append(']');

  }

  json.append('}').append("\r\n");

  QByteArray http = QByteArray()
                    .append("HTTP/1.0 200 Ok\r\n")
                    .append("Content-Type: text/json; charset=\"utf-8\"\r\n")
                    .append(QString("Content-Length: %1\r\n").arg(json.length() + 2))
                    .append("Access-Control-Allow-Origin: *\r\n")
                    .append("Access-Control-Allow-Headers: *\r\n")
                    .append("Origin: file://\r\n\r\n")        //! обязательно два!
                    .append(json).append("\r\n");

//  if(m_logger && m_logger->options().log_level >= sv::log::llDebug2)
//    *m_logger << sv::log::llDebug2 << sv::log::mtDebug << QString(http) << sv::log::endl;

  emit message(QString(http), sv::log::llDebug2, sv::log::mtDebug);

  return http;

}

QByteArray restapi::SvRestAPI::reply_ws_get(const HttpRequest &request)
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
//  replay.append(QString("Date: Wed, %1\r\n").arg(QDateTime::currentDateTimeUtc().toString("dd MMM yyyy hh:mm:ss t")));
  replay.append(QString("Upgrade: websocket\r\n"));
  replay.append(QString("Connection: Upgrade\r\n"));

  if(request.fields.contains("sec-websocket-key")) {

    QString key = request.fields.value("sec-websocket-key");
    key.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
//qDebug() << key;
    QByteArray hash64 = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1).toBase64();
    qDebug() << QString(hash64);

    replay.append(QString("Sec-WebSocket-Accept: %1\r\n\r\n").arg(QString(hash64)));

  }

  return replay;

}

QByteArray restapi::SvRestAPI::getHttpError(int errorCode, QString errorString)
{
  emit message(errorString, sv::log::llError, sv::log::mtError);

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
}

/** ********** EXPORT ************ **/
modus::SvAbstractInteract* create()
{
  modus::SvAbstractInteract* server = new restapi::SvRestAPI();
  return server;
}
