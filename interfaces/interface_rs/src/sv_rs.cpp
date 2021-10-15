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

    connect(m_port, &QSerialPort::readyRead, this, &SvRS::read);
    connect(p_io_buffer, &modus::IOBuffer::readyWrite, this, &SvRS::write);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void SvRS::read()
{
  p_io_buffer->input->mutex.lock();

  if(p_io_buffer->input->offset + m_port->bytesAvailable() > p_config->bufsize)
    p_io_buffer->input->reset();

//    /* ... the rest of the datagram will be lost ... */
  qint64 readed = m_port->read(&p_io_buffer->input->data[0], p_config->bufsize);
  emit message(QString(QByteArray((const char*)&p_io_buffer->input->data[0], readed).toHex()), sv::log::llDebug, sv::log::mtReceive);

  p_io_buffer->input->offset = readed;

  while(m_port->waitForReadyRead(m_params.grain_gap)) {

    if(p_io_buffer->input->offset + m_port->bytesAvailable() > p_config->bufsize)
      p_io_buffer->input->reset();

    /* ... the rest of the datagram will be lost ... */
    qint64 readed2 = m_port->read(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);

//qDebug() << QString(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed).toHex());
    emit message(QString(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed2).toHex()), sv::log::llDebug, sv::log::mtReceive);
    p_io_buffer->input->offset += readed2;

  }

  p_io_buffer->input->mutex.unlock();

  emit p_io_buffer->dataReaded(p_io_buffer->input);

}


void SvRS::write(modus::BUFF* buffer)
{
  QMutexLocker(&(buffer->mutex));

  if(!buffer->ready())
    return;

  bool written = m_port->write(&buffer->data[0], buffer->offset) > 0;
  m_port->flush();

  if(written)
    emit message(QString(QByteArray((const char*)&buffer->data[0], buffer->offset).toHex()), sv::log::llDebug, sv::log::mtSend);

  buffer->reset();

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
