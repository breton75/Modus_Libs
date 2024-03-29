﻿#include "sv_restapi_server.h"

//using namespace sv::log;


/** ********** SvWebServer ************ **/

restapi::SvRestAPI::SvRestAPI():
  modus::SvAbstractProvider(),
  m_server(new QTcpServer()),
  m_is_active(false)
{

}

restapi::SvRestAPI::~SvRestAPI()
{

}

bool restapi::SvRestAPI::configure(modus::ProviderConfig* config, modus::Configuration* configuration)
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
  if(!modus::SvAbstractProvider::bindSignal(signal))
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

  http::HttpRequest request = http::HttpRequest::parse(req);

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

  http::HttpRequest request;

  // разбираем заголовок запроса
  request.method   = QString(rawheader.at(0).trimmed());
  request.resourse = QString(rawheader.at(1).split('?').count() > 0 ? rawheader.at(1).split('?').at(0).trimmed() : rawheader.at(1));
  request.params   = QString(rawheader.at(1).split('?').count() > 1 ? rawheader.at(1).split('?').at(1).trimmed() : "");
  request.protocol = QString(rawheader.at(2).split('/').count() > 0 ? rawheader.at(2).split('/').at(0).trimmed() : rawheader.at(2));
  request.version  = QString(rawheader.at(2).split('/').count() > 1 ? rawheader.at(2).split('/').at(1).trimmed() : "");
}

QByteArray restapi::SvRestAPI::reply_http_get(const http::HttpRequest &request)
{
  return http::replyHttpGet(request, m_params.html_path, m_params.index_file);
}

QByteArray restapi::SvRestAPI::reply_http_get_params(const http::HttpRequest &request)
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

  QString entity  = "";
  QString task    = "";
  QString option  = "";
  QString data    = "";

  for(QString param: get_params) {

    if(param.indexOf('=') < 1)
      return getErr(404, QString("Неверный запрос. Неопределенный параметр %1").arg(param));

    QString field = param.left(param.indexOf('='));
    QString tag   = param.right(param.length() - param.indexOf('=') - 1);

    if(field.toLower().trimmed() == "entity")
      entity = tag;

    else if(field.toLower().trimmed() == "task")
      task = tag;

    else if(field.toLower().trimmed() == "option")
      option = tag;

    else if(field.toLower().trimmed() == "data")
      data = tag;

  }

  QByteArray json = getEntityData(entity, task, option, data);

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

  emit message(QString(http), sv::log::llDebug, sv::log::mtReply);

  return http;

}

QByteArray restapi::SvRestAPI::getEntityData(const QString& entity, const QString& task, const QString& option, const QString& data, const char separator)
{
  QByteArray result;

  switch (restapi::eto::EntitiesTable.value(entity, restapi::eto::noentity)) {

    case restapi::eto::signal:

      result = getSignalsData(task, option, data, separator);

      break;

    case restapi::eto::configuration:

      result = getConfigurationData(task, option, data, separator);

      break;

    default:
      result = "Не доступно в данной версии";
      break;

  }

  return result;

}

QByteArray restapi::SvRestAPI::getSignalsData(const QString& task, const QString& option, const QString& data, const char separator)
{
  QByteArray result;

  switch (restapi::eto::TasksTable.value(task, restapi::eto::notask)) {

    case restapi::eto::value:

      result = getSignalsValues(option, data, separator);

      break;

    case restapi::eto::json:

      result = "Не доступно в данной версии";
      break;

    default:
      result = "Не доступно в данной версии";
      break;

  }

  return result;

}

QByteArray restapi::SvRestAPI::getSignalsValues(const QString& option, const QString& data, const char separator)
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

  switch (restapi::eto::OptionsTable.value(option, restapi::eto::nooption)) {

    case restapi::eto::Options::byid:
    {
      QList<QString> ids = data.split(QChar(separator), QString::SkipEmptyParts);

      if(ids.isEmpty())
        errors.append("{\"value\": \"Неверный запрос значений сигналов. Список идентификаторов (data) пуст.\"},");

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

    case eto::Options::byname:
    {

      QList<QString> names = data.split(QChar(separator), QString::SkipEmptyParts);

      if(names.isEmpty())
        errors.append("{\"value\": \"Неверный запрос значений сигналов. Список имен сигналов (data) пуст.\"}");

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

QByteArray restapi::SvRestAPI::getConfigurationData(const QString& task, const QString& option, const QString& data, const char separator)
{
  return QByteArray(QString("Not avalable yet. %1 %2 %3 %4").arg(task,option,data).arg(separator).toUtf8());
}

QByteArray restapi::SvRestAPI::reply_http_post(const http::HttpRequest &request)
{
  QByteArray json   = QByteArray();

  QJsonParseError per;
  QJsonDocument jd = QJsonDocument::fromJson(request.data, &per); // запрос должен быть в формате JSON

  if(per.error != QJsonParseError::NoError) {

    return getHttpError(400, QString("Некорректный запрос на обновление данных.\n%1\n%2")
                        .arg(QString(request.data)).arg(per.errorString()));
  }

  QJsonObject jo = jd.object();
  if(!jo.contains(P_ENTITY) || !jo.value(P_ENTITY).isString() || !jo.contains(P_DATA) || !jo.value(P_DATA).isObject()) {

     return getHttpError(400, QString("Некорректный запрос на обновление данных.\n%1\n%2")
                         .arg(QString(request.data)).arg(per.errorString()));

  }

  if((jo.value(P_ENTITY).toString() == P_SIGNAL) || (jo.value(P_ENTITY).toString() == P_SIGNALS)) {

    json = setSignalValues(jo.value(P_DATA).toObject());

  }
  else if(jo.value(P_ENTITY).toString() == P_EVENTLOG) {

//    json = setEventlog(jo.value(P_DATA).toObject());

  }


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

  emit message(QString(http), sv::log::llDebug, sv::log::mtReply);

  return http;

}

QByteArray restapi::SvRestAPI::setSignalValues(const QJsonObject& jo)
{
  QByteArray result   = QByteArray();
  result.append('{');

  QByteArray errors = QByteArray();

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


  if(!errors.isEmpty()) {

    if(errors.endsWith(',')) errors.chop(1);

    result.append(QString("\"errors\":[").toUtf8()).append(errors).append(']');

  }

  result.append('}').append("\r\n");

  return result;

}

/*QByteArray restapi::SvRestAPI::setEventlog(const QJsonObject& jo)
{
  QByteArray result   = QByteArray();
  result.append('{');

  QByteArray errors = QByteArray();

  if(jo.contains(P_LOGGERS) && jo.value(P_SIGNALS).toArray())
  QJsonArray ja = jo.value(P_SIGNALS).toArray();

  for(QJsonValue jv: ja) {

    if(!jv.isObject())
      continue;

    QJsonObject o = jv.toObject();

  }
}
*/

QByteArray restapi::SvRestAPI::reply_ws_get(const http::HttpRequest &request)
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


/** ********** EXPORT ************ **/
modus::SvAbstractProvider* create()
{
  modus::SvAbstractProvider* server = new restapi::SvRestAPI();
  return server;
}
