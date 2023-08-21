#include "sv_rs.h"

SvRS::SvRS()
{
}

bool SvRS::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = SerialParams::fromJsonString(p_config->interface.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool SvRS::start()
{
  try {

    m_port = new QSerialPort();

    m_port->setPortName(m_params.portname);
    m_port->setBaudRate(m_params.baudrate);
    m_port->setDataBits(m_params.databits);
    m_port->setFlowControl(m_params.flowcontrol);
    m_port->setParity(m_params.parity);
    m_port->setStopBits(m_params.stopbits);

    if(!m_port->open(QIODevice::ReadWrite))
      throw SvException(m_port->errorString());

    if(m_params.dtr_control) {

      m_port->write(QByteArray::fromHex("00"));
      m_port->setDataTerminalReady(false);
    }

    connect(m_port, &QSerialPort::readyRead, this, &SvRS::read);
    connect(p_io_buffer, &modus::IOBuffer::readyWrite, this, &SvRS::write);

    m_gap_timer = new QTimer;
    m_gap_timer->setInterval(m_params.grain_gap);
    m_gap_timer->setSingleShot(true);
    connect(m_gap_timer, &QTimer::timeout, this, &SvRS::newData);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void SvRS::read()
{
  m_gap_timer->stop();

  if(p_io_buffer->input->isReady())
    p_io_buffer->input->reset();

  p_io_buffer->input->mutex.lock();

  if(p_io_buffer->input->offset + m_port->bytesAvailable() > p_config->bufsize)
    p_io_buffer->input->reset();

//    /* ... the rest of the datagram will be lost ... */
  qint64 readed = m_port->read(&p_io_buffer->input->data[0], p_config->bufsize);

  emit_message(QByteArray((const char*)&p_io_buffer->input->data[0], readed), sv::log::llDebug, sv::log::mtReceive);

  p_io_buffer->input->offset += readed;

  p_io_buffer->input->mutex.unlock();

  m_gap_timer->start(m_params.grain_gap);

}

void SvRS::newData()
{
  QMutexLocker(&p_io_buffer->input->mutex);

  p_io_buffer->input->setReady(true);
  emit p_io_buffer->dataReaded(p_io_buffer->input);
}

void SvRS::write(modus::BUFF* buffer)
{
  if(!buffer->isReady())
    return;

  buffer->mutex.lock();

  if(m_params.dtr_control)
    m_port->setDataTerminalReady(true);

  bool written = m_port->write(&buffer->data[0], buffer->offset) > 0;
  m_port->flush();

  if(written)
    emit_message(QByteArray((const char*)&buffer->data[0], buffer->offset), sv::log::llDebug, sv::log::mtSend);

  buffer->reset();

  if(m_params.dtr_control)
    m_port->setDataTerminalReady(false);

  buffer->mutex.unlock();

}

void SvRS::emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type)
{
  QString msg = "";

  //! The append() function is typically very fast
  switch (m_params.fmt) {
    case modus::HEX:
      msg.append(bytes.toHex());
      break;

    case modus::ASCII:
      msg.append(bytes);
      break;

    case modus::DATALEN:
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
  modus::SvAbstractInterface* device = new SvRS();
  return device;
}

const char* getDefaultParams()
{
  return "{ \"portname\": \"ttyS6\", \"baudrate\": 19200, \"databits\": 8, \"parity\": 0, \"stopbits\": 2, \"flowcontrol\": 0 }";
}

const char* getName()
{
  return "Драйвер для работы с COM портами";
}

const char* getDescription()
{
  return "Драйвер для работы с COM портами";
}
