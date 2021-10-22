#include "zn_writer.h"

QMutex mutex;

QSemaphore sem(1);

zn1::ZNWriter::ZNWriter():
  m_socket(nullptr),
  m_state_signal(nullptr),
  m_zn_state()
{

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

bool zn1::ZNWriter::bindSignal(modus::SvSignal* signal, modus::SignalBinding binding)
{
  bool r = modus::SvAbstractStorage::bindSignal(signal, binding);

  if(r) {

    if(binding.mode == modus::Master) {

      if(signal->config()->type.toLower() == TYPE_STAT) {

        if(m_state_signal) {

          p_last_error = TOO_MUCH(p_config->name, TYPE_STAT);
//          qDebug() << QString("Second: %1").arg(signal->config()->name);
          return false;
        }
        else {
//          qDebug() << QString("First: %1").arg(signal->config()->name);
          m_state_signal = signal;

        }
      }
    }
  }

  return  true;

}

void zn1::ZNWriter::start()
{
//  m_timer = new QTimer;
//  connect(m_timer, &QTimer::timeout, this, &ZNWriter::checkupSignals);
//  m_timer->start(DEFAULT_INTERVAL);

  m_socket = new sv::tcp::Client(); // обязательно создаем здесь, чтобы объект принадлежал этому потоку

  m_authorized = false;

}

//void zn1::ZNWriter::checkupSignals()
//{
//  if(m_state_signal)
//    m_state_signal->approve(false);
//}

void zn1::ZNWriter::setState(int writeState, int authorization, int connectionState)
{
  m_zn_state.c = connectionState;
  m_zn_state.a = authorization;
  m_zn_state.w = writeState;

  if(m_state_signal)
    m_state_signal->setValue(m_zn_state.state());

//  m_timer->start();

}

void zn1::ZNWriter::write()
{
  try {

    if(!m_socket)
      throw SvException("Panic! m_socket is not defined!");

    { //! 1. физическое подключение к хосту

      if(m_socket->state() != QAbstractSocket::ConnectedState) {

        m_authorized = false;
//        setState(0x07, STATE_FAIL_NO_WRITING, STATE_FAIL_NO_AUTHORITY, STATE_FAIL_NO_CONNECTION);
        setState(0, 0, 0);

        emit message(QString("Подключаюсь к защищенному хранилищу %1:%2")
                     .arg(m_params.host.toString()).arg(m_params.port), sv::log::llDebug2, sv::log::mtDebug1);

        if(!m_socket->connectTo(m_params.host, m_params.port))
          throw SvException(QString("Не удалось подключиться к защищенному накопителю по адресу %1:%2.\n%3")
                            .arg(m_params.host.toString()).arg(m_params.port).arg(m_socket->lastError()), 0);

      }
    } //! ~физическое подключение к хосту

    { //! 2. авторизация на устройстве

      if((m_socket->state() == QAbstractSocket::ConnectedState) && !m_authorized) {

        setState(0, 0, 1);

        zn1::AuthorizeRequest connreq(m_params.zone, m_params.pass);

        QByteArray r = connreq.toByteArray();

        emit message(QString("Запрос на авторизацию: %1").arg(QString(r.toHex())), sv::log::llDebug2, sv::log::mtRequest);

        m_socket->write(r);

        if(!m_socket->waitForReadyRead(m_params.interval))
          throw SvException(QString("Ошибка авторизации на защищенном накопителе. Нет ответа."), 1);

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
                                .arg(ReplyCodeMap.value(static_cast<zn1::ReplyCode>(reply.result), "Ух ты! Как ты это сделал?")), 1);
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

    setState(0, 0, e.code); //0x07, STATE_FAIL_NO_WRITING, STATE_FAIL_NO_AUTHORITY, STATE_FAIL_NO_CONNECTION);

  }

  { //! 3. запись данных

    if(m_authorized && (m_socket->state() == QAbstractSocket::ConnectedState)) {

      setState(0,1,1);

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

              setState(1,1,1);
              break;
            }

            default:

              emit message(QString("Ошибка записи: %1").arg(ReplyCodeMap.value(static_cast<zn1::ReplyCode>(reply.result), "Ух ты! Как ты это сделал?")),
                           sv::log::llDebug, sv::log::mtFail);

              setState(0,1,1); //0x04, STATE_FAIL_NO_WRITING);
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
