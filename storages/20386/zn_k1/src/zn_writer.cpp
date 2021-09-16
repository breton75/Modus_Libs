#include "zn_writer.h"

QMutex mutex;

QSemaphore sem(1);

zn1::ZNWriter::ZNWriter():
  m_zn_state(QMap<QString, modus::SvSignal*>())
{
//  moveToThread(this);
//  moveToThread(thread()); //!
}

bool zn1::ZNWriter::configure(modus::StorageConfig* config)
{
  try {

    p_config = config;

    m_params = zn1::Params::fromJsonString(p_config->params);


  }
  catch(SvException& e) {

    p_last_error = e.error;
    return false;
  }

  return true;
}

void zn1::ZNWriter::disposeInputSignal  (modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void zn1::ZNWriter::disposeOutputSignal (modus::SvSignal* signal)
{
  m_zn_state.insert(signal->config()->tag.toLower(), signal);
}

//bool zn1::ZNWriter::bindSignal(modus::SvSignal* signal)
//{

//    qDebug() << QString("run. cal thread %1, current thread: %2").arg(reinterpret_cast<quint64>(this->thread()))
//                 .arg(reinterpret_cast<quint64>(currentThread()));
//  connect(signal, &modus::SvSignal::updated, this, &ZNWriter::signalUpdated);

//  return modus::SvAbstractStorage::bindSignal(signal);
//}

void zn1::ZNWriter::start()
{
//  connect(&tm, &QTimer::timeout, this, &zn1::ZNWriter::write);
  tm.start(m_params.interval);

  m_socket = new sv::tcp::Client(); // обязательно создаем здесь, чтобы объект принадлежал этому потоку
//  m_socket->moveToThread(this); - так не работает
//  m_socket->setFlags(0);

  m_authorized = false;
}

void zn1::ZNWriter::setState(int doChangeFlags, const QString& writeState, const QString& authorization, const QString& connectionState)
{
  if((doChangeFlags & 0x01) && m_zn_state.contains(TAG_CONNECTION_STATE))
     m_zn_state.value(TAG_CONNECTION_STATE)->setValue(connectionState);

  if((doChangeFlags & 0x02) && m_zn_state.contains(TAG_AUTHORIZATION))
     m_zn_state.value(TAG_AUTHORIZATION)->setValue(authorization);

  if((doChangeFlags & 0x04) && m_zn_state.contains(TAG_WRITE_STATE))
     m_zn_state.value(TAG_WRITE_STATE)->setValue(writeState);
}

void zn1::ZNWriter::write()
{
  try {

    { //! 1. физическое подключение к хосту

      if(m_socket->state() != QAbstractSocket::ConnectedState) {

        m_authorized = false;
        setState(0x07, STATE_FAIL_NO_WRITING, STATE_FAIL_NO_AUTHORITY, STATE_FAIL_NO_CONNECTION);

        emit message(QString("Подключаюсь к защищенному хранилищу %1:%2")
                     .arg(m_params.host.toString()).arg(m_params.port), sv::log::llDebug2, sv::log::mtDebug1);

        if(!m_socket->connectTo(m_params.host, m_params.port))
          throw SvException(QString("Не удалось подключиться к защищенному накопителю по адресу %1:%2.\n%3")
                            .arg(m_params.host.toString()).arg(m_params.port).arg(m_socket->lastError()), 1);

      }
    } //! ~физическое подключение к хосту

    { //! 2. авторизация на устройстве

      if((m_socket->state() == QAbstractSocket::ConnectedState) && !m_authorized) {

        setState(0x01);

        zn1::AuthorizeRequest connreq(m_params.zone, m_params.pass);

        QByteArray r = connreq.toByteArray();

        emit message(QString("Запрос на авторизацию: %1").arg(QString(r.toHex())), sv::log::llDebug2, sv::log::mtRequest);

        m_socket->write(r);

        if(!m_socket->waitForReadyRead(m_params.interval))
          throw SvException(QString("Ошибка авторизации на защищенном накопителе. Нет ответа."), 2);

        else {

          // анализируем полученный ответ
          QByteArray r = m_socket->readAll();

          zn1::AuthorizeReply reply = zn1::AuthorizeReply::parse(r);

          emit message(QString("Ответ на запрос авторизации: %1\n"
                               "len: %2, reply code: %3, request code: %4, result: %5, addition len: %6 bytes")
                          .arg(QString(r.toHex())).arg(reply.length).arg(reply.reply_code).arg(reply.request_code)
                          .arg(reply.result).arg(reply.additional.length()),
                       sv::log::llDebug2, sv::log::mtDebug);

          switch (static_cast<ReplyCode>(reply.result)) {

            case ReplyCode::Success:

              m_authorized = true;
              emit message(QString("Успешная авторизация"), sv::log::llDebug, sv::log::mtSuccess);
              break;

            case ReplyCode::Failure:
            case ReplyCode::NotSupportedCmd:
            case ReplyCode::WrongZoneName:
            case ReplyCode::WrongPassword:
            case ReplyCode::AlreadyInUse:

              throw SvException(QString("Ошибка авторизации: %1")
                                .arg(ReplyCodeMap.value(static_cast<zn1::ReplyCode>(reply.result), "Ух ты! Как ты это сделал?")), 2);
              break;

            default:
              break;
          }
        }
      }
    } //! ~авторизация на устройстве
  }
  catch(SvException& e) {

    emit message(e.error, sv::log::llDebug, sv::log::mtError);
    m_socket->disconnectFromHost();
    m_authorized = false;

    switch (e.code) {
      case 1:
        setState(0x07, STATE_FAIL_NO_WRITING, STATE_FAIL_NO_AUTHORITY, STATE_FAIL_NO_CONNECTION);
        break;

      case 2:
        setState(0x06, STATE_FAIL_NO_WRITING, STATE_FAIL_NO_AUTHORITY);
        break;

      default:
        break;
    }
  }

  { //! 3. запись данных

    if(m_authorized && (m_socket->state() == QAbstractSocket::ConnectedState)) {

      setState(0x03);

      // если попали сюда, значит есть подключение и авторизация. можно писать данные
      if(!bunches.isEmpty()) {

        QByteArray data = QByteArray();

        foreach (Bunch* b, bunches)
          data.append(b->toByteArray());

        QByteArray pack = QByteArray();
        QDataStream stream(&pack, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);

        stream << quint32(data.length())
               << quint16(CMD_WRITE);

        pack.append(data);

        m_socket->write(pack);

        emit message(QString("Запись %1 байт в зону %2").arg(data.length()).arg(m_params.zone),
                     sv::log::llDebug2, sv::log::mtDebug);

        if(!m_socket->waitForReadyRead(m_params.interval))
          emit message(QString("Ошибка записи данных в  защищенный накопитель. Нет ответа."), sv::log::llDebug, sv::log::mtFail);

        else {

          QByteArray r = m_socket->readAll();
          zn1::WriteReply reply = zn1::WriteReply::parse(r);

          emit message(QString("Результат записи: %1\nlen: %2, reply code: %3, request code: %4, result: %5, addition: %6 bytes")
                            .arg(QString(r.toHex())).arg(reply.length).arg(reply.reply_code).arg(reply.request_code)
                       .arg(reply.result).arg(reply.additional.length()),
                       sv::log::llDebug2, sv::log::mtDebug);

          switch (static_cast<ReplyCode>(reply.result )) {

            case ReplyCode::Success:
            {
              while(!bunches.isEmpty())
                delete bunches.dequeue();

              emit message(QString("%1 байт успешно записаны в зону %2").arg(pack.length()).arg(m_params.zone), sv::log::llDebug, sv::log::mtSuccess);

              setState(0x04);
              break;
            }

            default:

              emit message(QString("Ошибка записи: %1").arg(ReplyCodeMap.value(static_cast<zn1::ReplyCode>(reply.result), "Ух ты! Как ты это сделал?")),
                           sv::log::llDebug, sv::log::mtFail);

              setState(0x04, STATE_FAIL_NO_WRITING);
              break;

          }
        }
      }
    }
  } //! ~запись данных
}


void zn1::ZNWriter::signalUpdated(modus::SvSignal* signal)
{
//  emit message(QString("1. m_mutex.lock(). bunches.count(): %1").arg(bunches.count()), sv::log::llDebug, sv::log::mtDebug1);

  if(bunches.isEmpty() || (bunches.last()->state() != Bunch::Underway)) {

    if(bunches.count() >= int(m_params.queue_len))
      delete bunches.dequeue();

    bunches.enqueue(new Bunch(QDateTime::currentMSecsSinceEpoch()));

    emit message(QString("Bunch queued. Bunch count: %1").arg(bunches.count()), sv::log::llDebug2, sv::log::mtDebug);

  }

  if(bunches.last()->state() == Bunch::Underway)
  {
    Record p(signal->lastUpdate().toMSecsSinceEpoch(), signal->config()->tag, signal->value().toByteArray());
    QByteArray r = p.toByteArray();
    bunches.last()->appendRecord(r);

    emit message(QString("%1 bytes appended to bunch. Bunch size: %2, records: %3")
                 .arg(r.length()).arg(bunches.last()->length()).arg(bunches.last()->recordCount()), sv::log::llDebug2, sv::log::mtDebug);
  }

  if(bunches.last()->length() >= m_params.write_buf) {

    bunches.last()->setState(Bunch::Ready);

    write();

  }

//  emit message(QString("Bunch data size %1").arg(m_bunches.last()->data.length()), sv::log::llDebug2, sv::log::mtDebug);

}

/** ********** EXPORT ************ **/
modus::SvAbstractStorage* create()
{
  modus::SvAbstractStorage* storage = new zn1::ZNWriter();
  return storage;
}
