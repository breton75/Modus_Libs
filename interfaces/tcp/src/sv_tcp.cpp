#include "sv_tcp.h"

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

      m_client = new QTcpSocket; // tcp::Client(m_params.host, m_params.port, m_params.fmt);

      connectToServer();

      connect(m_client,     &QTcpSocket::readyRead,       this, &SvTcp::read        );
      connect(m_client,     &QTcpSocket::connected,       this, &SvTcp::connected   );
      connect(m_client,     &QTcpSocket::disconnected,    this, &SvTcp::disconnected);
      connect(p_io_buffer,  &modus::IOBuffer::readyWrite, this, &SvTcp::write       );

      m_gap_timer = new QTimer;
      m_gap_timer->setInterval(m_params.grain_gap);
      m_gap_timer->setSingleShot(true);
      connect(m_gap_timer, &QTimer::timeout, this, &SvTcp::newData);

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

      if(!m_client->waitForConnected(m_params.connect_timeout))
        throw SvException(m_client->errorString());

      emit message(QString("Успешное подключение к серверу"), lldbg, mtscc);

    }

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    emit message(QString("Ошибка подключения к серверу: %1").arg(p_last_error), llerr, mterr);
    return false;

  }
}

void SvTcp::read()
{
  m_gap_timer->stop();

  if(p_io_buffer->input->isReady())
    p_io_buffer->input->reset();

  p_io_buffer->input->mutex.lock();

  if(p_io_buffer->input->offset + m_client->bytesAvailable() > p_config->bufsize)
    p_io_buffer->input->reset();

  qint64 readed = m_client->read(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);

  emit_message(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed), sv::log::llDebug, sv::log::mtReceive);

  p_io_buffer->input->offset += readed;

  p_io_buffer->input->mutex.unlock();

  m_gap_timer->start(m_params.grain_gap);

}

void SvTcp::newData()
{
  QMutexLocker(&p_io_buffer->input->mutex);

  p_io_buffer->input->setReady(true);
  emit p_io_buffer->dataReaded(p_io_buffer->input);
}

void SvTcp::write(modus::BUFF* buffer)
{
  if(!buffer->isReady())
    return;

  if(!connectToServer())
    return;

  buffer->mutex.lock();

  bool written = m_client->write((const char*)&buffer->data[0], buffer->offset) > 0;
  m_client->flush();

  if(written)
    emit_message(QByteArray((const char*)&buffer->data[0], buffer->offset), sv::log::llDebug, sv::log::mtSend);

  buffer->reset();

  buffer->mutex.unlock();

}

void SvTcp::emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type)
{
  QString msg = "";

  //! The append() function is typically very fast
  switch (m_params.fmt) {
    case apak::HEX:
      msg.append(bytes.toHex());
      break;

    case apak::ASCII:
      msg.append(bytes);
      break;

    case apak::DATALEN:
      msg = QString("%1 байт %2").arg(bytes.length()).arg(type == sv::log::mtSend ? "отправлено" : type == sv::log::mtReceive ? "принято" : "");
      break;

    default:
      return;
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
