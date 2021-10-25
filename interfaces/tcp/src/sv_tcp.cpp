﻿#include "sv_tcp.h"

SvTcp::SvTcp()
{
}

bool SvTcp::configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = tcp::Params::fromJsonString(p_config->interface.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool SvTcp::start()
{
  try {

    if(m_params.mode == P_MODE_CLIENT) {

      m_client = new tcp::Client(m_params.host, m_params.port, m_params.fmt);

      if(!connectToServer())
        throw SvException(p_last_error);

      connect(m_client,     &tcp::Client::message,        this, &SvTcp::message     );
      connect(m_client,     &tcp::Client::readyRead,      this, &SvTcp::read        );
      connect(m_client,     &tcp::Client::connected,      this, &SvTcp::connected   );
      connect(m_client,     &tcp::Client::disconnected,   this, &SvTcp::disconnected);
      connect(p_io_buffer,  &modus::IOBuffer::readyWrite, this, &SvTcp::write       );

      m_gap_timer.setInterval(m_params.grain_gap);
      m_gap_timer.setSingleShot(true);
      connect(&m_gap_timer, &QTimer::timeout, this, &SvTcp::newData);

      return true;

    }
    else {

      throw SvException(QString("Режим работы \"%1\" не поддерживается").arg(m_params.mode));

    }

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void SvTcp::socketError(QAbstractSocket::SocketError err)
{
  emit message(tcp::SocketErrors.value(err, QString("Неизвестная ошибка подключения")), lldbg, mterr);
}

void SvTcp::stateChanged(QAbstractSocket::SocketState state)
{
  emit message(tcp::SocketStates.value(state, QString("Неизвестное состояние подключения")), sv::log::llDebug, sv::log::mtConnection);
}

bool SvTcp::connectToServer()
{
  try {

    if (m_client->state() != QAbstractSocket::ConnectedState) {

      m_client->connectToHost(m_params.host, m_params.port);

      if(!m_client->waitForConnected(m_params.timeout))
        throw SvException(m_client->errorString());

    }

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void SvTcp::read()
{
  m_gap_timer.stop();

  p_io_buffer->input->mutex.lock();

  if(p_io_buffer->input->offset + m_client->bytesAvailable() > p_config->bufsize)
    p_io_buffer->input->reset();

  qint64 readed = m_client->read(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);

  emit_message(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed), sv::log::llDebug, sv::log::mtReceive);

  !!!! p_io_buffer->input->offset += readed;

  p_io_buffer->input->mutex.unlock();

  m_gap_timer.start(m_params.grain_gap);

}

void SvTcp::newData()
{
  emit p_io_buffer->dataReaded(p_io_buffer->input);
}

void SvTcp::write(modus::BUFF* buffer)
{
  if(!buffer->ready())
    return;

  if(m_client->state() != QAbstractSocket::ConnectedState) {

    if(!connectToServer) {

      emit message(p_last_error, sv::log::llError, sv::log::mtError);
      emit error();

      return;
    }
  }

  buffer->mutex.lock();

  bool written = m_client->write((const char*)&buffer->data[0], buffer->offset) > 0;
  m_client->flush();

  if(written)
    emit message(QString(QByteArray((const char*)&buffer->data[0], buffer->offset).toHex()), sv::log::llDebug, sv::log::mtSend);

  buffer->reset();

  buffer->mutex.unlock();

}

void SvTcp::emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type)
{
  QString msg = "";

  //! The append() function is typically very fast
  switch (m_params.fmt) {
    case tcp::HEX:
      msg.append(bytes.toHex());
      break;

    case tcp::ASCII:
      msg.append(bytes);
      break;

    case tcp::ASCII:
      msg = QString("%1 байт").arg(bytes.length());
      break;

    default:
      break;
  }

  emit message(msg, level, type);

}

/** ********** EXPORT ************ **/
modus::SvAbstractInterface* create()
{
  modus::SvAbstractInterface* device = new SvTcp();
  return device;
}

const char* getDefaultParams()
{
  return "{ \"host\": \"172.16.4.11\", \"recv_port\": 6001, \"send_port\": 5001 }";
}

const char* getName()
{
  return "Драйвер для работы с UDP";
}

const char* getDescription()
{
  return "Драйвер для работы с UDP";
}
