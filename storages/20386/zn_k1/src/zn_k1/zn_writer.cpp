#include "zn_writer.h"


zn1::ZNWriter::ZNWriter()
{

}

bool zn1::ZNWriter::configure(modus::StorageConfig* config)
{
  try {

    p_config = config;

    m_params = zn1::Params::fromJsonString(p_config->params);

    m_socket = new sv::tcp::Client();


  }
  catch(SvException& e) {

    p_last_error = e.error;
    return false;
  }
}

bool zn1::ZNWriter::bindSignal(modus::SvSignal* signal)
{
  modus::SvAbstractStorage::bindSignal(signal);

  connect(signal, &modus::SvSignal::updated, this, &ZNWriter::signalUpdated);
}

void zn1::ZNWriter::run() override
{
  p_is_active = true;

  while (p_is_active) {


    QByteArray packet = QByteArray();

    while(!m_queue.isEmpty()) {

      modus::SvSignal* signal = m_queue.takeFirst();

      packet.append(signal->lastUpdate().toMSecsSinceEpoch())
            .append(signal->config()->tag.length())
            .append(signal->config()->tag.toUtf8())
            .append(signal->value().toByteArray().length())
            .append(signal->value().toByteArray());
    }

    if(m_socket->state() != QAbstractSocket::ConnectedState) {

      try {

        if(!m_socket->connectTo(m_params.host, m_params.port))
          throw SvException(QString("Не удалось подключиться к защищенному накопителю по адресу %1:%2.\n%3")
                            .arg(m_params.host).arg(m_params.port).arg(m_socket->lastError()));

        else {

          zn1::ConnectRequest connreq(m_params.zone, m_params.pass);

          QByteArray r = connreq.toByteArray();

          emit message(QString(r.toHex()), sv::log::llDebug, sv::log::mtRequest);

          m_socket->write(r);
          m_socket->flush();

          if(!m_socket->waitForReadyRead(m_params.interval))
            throw SvException(QString("Ошибка подключения к защищенному накопителю. Нет ответа."));

          else {

            // анализируем полученный ответ
            QByteArray r = m_socket->readAll();

            emit message(QString(r.toHex()), sv::log::llDebug, sv::log::mtReply);

            zn1::ConnectReply reply = zn1::ConnectReply::parse(r);

            emit message(QString("len: %1, reply code: %2, request code: %3, result: %4, addition len: %5 bytes")
                            .arg(reply.length).arg(reply.reply_code).arg(reply.request_code).arg(reply.result).arg(reply.additional.length()),
                         sv::log::llDebug2, sv::log::mtParse);

            switch (static_cast<ReplyCode>(reply.result)) {

              case ReplyCode::Success:

                break;

              case ReplyCode::Failure:
              case ReplyCode::NotSupportedCmd:
              case ReplyCode::WrongZoneName:
              case ReplyCode::WrongPassword:
              case ReplyCode::AlreadyInUse:

                throw SvException(QString("Ошибка подключения: %1").arg(ReplyCodeMap.value(reply.result, "Ух ты! Как ты это сделал?")));
                break;

              default:
                break;
            }
          }
        }
      }
      catch(SvException& e) {

        emit message(e.error, sv::log::llDebug, sv::log::mtError);
        m_socket->disconnectFromHost();

      }
    }


    if(m_socket->state() == QAbstractSocket::ConnectedState) {

      // если попали сюда, значит подключение есть и можно писать данные
      m_socket->write(packet);
      m_socket->flush();

      if(!m_socket->waitForReadyRead(m_params.interval))
        emit message(QString("Ошибка записи данных в  защищенный накопитель. Нет ответа."), sv::log::llDebug, sv::log::mtFail);

      else {

        zn1::ConnectReply reply = zn1::ConnectReply::parse(m_socket->readAll());

        emit message(QString("len: %1, reply code: %2, request code: %3, result: %4, addition len: %5 bytes")
                        .arg(reply.length).arg(reply.reply_code).arg(reply.request_code).arg(reply.result).arg(reply.additional.length()),
                     sv::log::llDebug2, sv::log::mtReply);

        emit message(QString("%1 байт данных успешно записаны в хранилище").arg(packet.size()), sv::log::llDebug, sv::log::mtReply);

      }
    }

    msleep(m_params.interval);

  }
}


void zn1::ZNWriter::signalUpdated(modus::SvSignal* signal)
{
  m_queue.append(signal);
}
