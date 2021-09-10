#include "zn_writer.h"

#include <QDataStream>

zn1::ZNWriter::ZNWriter()
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

bool zn1::ZNWriter::bindSignal(modus::SvSignal* signal)
{
  connect(signal, &modus::SvSignal::updated, this, &ZNWriter::signalUpdated);

  return modus::SvAbstractStorage::bindSignal(signal);
}

void zn1::ZNWriter::run()
{
  p_is_active = true;

  m_socket = new sv::tcp::Client(); // обязательно создаем здесь, чтобы объект принадлежал этому потоку
//  m_socket->moveToThread(this); - так не работает
  m_socket->setFlags(0);

  m_authorized = false;
  while (p_is_active) {

    try {

      { //! 1. осуществляем физическое подключение к хосту

        if(m_socket->state() != QAbstractSocket::ConnectedState) {

          m_authorized = false;

          if(!m_socket->connectTo(m_params.host, m_params.port))
            throw SvException(QString("Не удалось подключиться к защищенному накопителю по адресу %1:%2.\n%3")
                              .arg(m_params.host.toString()).arg(m_params.port).arg(m_socket->lastError()));

        }
      } //! ~1

      { //! 2. авторизуемся на устройстве

        if((m_socket->state() == QAbstractSocket::ConnectedState) && !m_authorized) {

          zn1::AuthorizeRequest connreq(m_params.zone, m_params.pass);

          QByteArray r = connreq.toByteArray();

          emit message(QString(r.toHex()), sv::log::llDebug, sv::log::mtRequest);

          m_socket->write(r);

          if(!m_socket->waitForReadyRead(m_params.interval))
            throw SvException(QString("Ошибка авторизации на защищенном накопителе. Нет ответа. %1").arg(m_params.interval));

          else {

            // анализируем полученный ответ
            QByteArray r = m_socket->readAll();

            emit message(QString(r.toHex()), sv::log::llDebug, sv::log::mtReply);

            zn1::AuthorizeReply reply = zn1::AuthorizeReply::parse(r);

            emit message(QString("len: %1, reply code: %2, request code: %3, result: %4, addition len: %5 bytes")
                            .arg(reply.length).arg(reply.reply_code).arg(reply.request_code).arg(reply.result).arg(reply.additional.length()),
                         sv::log::llDebug2, sv::log::mtParse);

            switch (static_cast<ReplyCode>(reply.result)) {

              case ReplyCode::Success:

                m_authorized = true;
                emit message(QString("Успешная авторизация"), sv::log::llDebug, sv::log::mtLogin);
                break;

              case ReplyCode::Failure:
              case ReplyCode::NotSupportedCmd:
              case ReplyCode::WrongZoneName:
              case ReplyCode::WrongPassword:
              case ReplyCode::AlreadyInUse:

                throw SvException(QString("Ошибка авторизации: %1")
                                  .arg(ReplyCodeMap.value(static_cast<zn1::ReplyCode>(reply.result), "Ух ты! Как ты это сделал?")));
                break;

              default:
                break;
            }
          }
        }
      } //! ~2

      { //! 3. запись данных

        if(m_authorized && (m_socket->state() == QAbstractSocket::ConnectedState)) {

          // если попали сюда, значит есть подключение и авторизация. можно писать данные
          m_mutex.lock();

          if(!m_bunches.isEmpty()) {

            foreach (Bunch* b, m_bunches)
              b->setState(Bunch::Ready);

            while(!m_bunches.isEmpty() && (m_bunches.first()->state != Bunch::Undelivered)) {

              quint64 sz = m_socket->write(m_bunches.first()->toByteArray());

              emit message(QString("Пытаюсь записать %1 байт в зону %2").arg(sz).arg(m_params.zone), sv::log::llDebug2, sv::log::mtSuccess);

              if(!m_socket->waitForReadyRead(m_params.interval))
                emit message(QString("Ошибка записи данных в  защищенный накопитель. Нет ответа."), sv::log::llDebug, sv::log::mtFail);

              else {

                zn1::AuthorizeReply reply = zn1::AuthorizeReply::parse(m_socket->readAll());

                emit message(QString("len: %1, reply code: %2, request code: %3, result: %4, addition len: %5 bytes")
                                .arg(reply.length).arg(reply.reply_code).arg(reply.request_code).arg(reply.result).arg(reply.additional.length()),
                             sv::log::llDebug2, sv::log::mtReply);

                switch (static_cast<ReplyCode>(reply.result )) {

                  case ReplyCode::Success:
                  {
                    delete m_bunches.dequeue();

                    emit message(QString("%1 байт успешно записаны в зону %2").arg(sz).arg(m_params.zone), sv::log::llDebug, sv::log::mtReply);

                    break;
                  }

                  default:

                    m_bunches.first()->setState(Bunch::Undelivered);

                    emit message(QString("Ошибка записи: %1").arg(ReplyCodeMap.value(static_cast<zn1::ReplyCode>(reply.result), "Ух ты! Как ты это сделал?")),
                                 sv::log::llDebug, sv::log::mtFail);
                    break;

                }
              }
            }
          }

          m_mutex.unlock();
        }
      } //! ~3

    }
    catch(SvException& e) {

      emit message(e.error, sv::log::llDebug, sv::log::mtError);
      m_socket->disconnectFromHost();
      m_authorized = false;
    }

    msleep(m_params.interval);

  }
}


void zn1::ZNWriter::signalUpdated(modus::SvSignal* signal)
{
  m_mutex.lock();

  if(m_bunches.isEmpty() || (m_bunches.last()->state != Bunch::Underway)) {

    if(m_bunches.count() >= m_params.queue_len)
      delete m_bunches.dequeue();

    m_bunches.enqueue(new Bunch(QDateTime::currentMSecsSinceEpoch()));

  }

  else //if(m_records.last()->state == Record::Underway)
  {
    Record p(signal->lastUpdate().toMSecsSinceEpoch(), signal->config()->tag, signal->value().toByteArray());
    m_bunches.last()->data.append(p.toByteArray());
  }

  m_mutex.unlock();

}

/** ********** EXPORT ************ **/
modus::SvAbstractStorage* create()
{
  modus::SvAbstractStorage* storage = new zn1::ZNWriter();
  return storage;
}
