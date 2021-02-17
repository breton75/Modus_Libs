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

    m_port = new QSerialPort();
    m_port->setPortName(m_params.portname);
    m_port->setBaudRate(m_params.baudrate);
    m_port->setDataBits(m_params.databits);
    m_port->setFlowControl(m_params.flowcontrol);
    m_port->setParity(m_params.parity);
    m_port->setStopBits(m_params.stopbits);

    if(!m_port->open(QIODevice::ReadWrite))
      throw SvException(m_port->errorString());

    m_port->moveToThread(this);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void SvRS::run()
{
  p_is_active = true;

  while(p_is_active) {

    while(m_port->waitForReadyRead(p_config->interface.buffer_reset_interval) && p_is_active) {

      p_io_buffer->input->mutex.lock();

      if(p_io_buffer->input->offset > p_config->bufsize)
        p_io_buffer->input->reset();

      p_io_buffer->input->offset += m_port->read((char*)(&p_io_buffer->input->data[p_io_buffer->input->offset]), p_config->bufsize - p_io_buffer->input->offset);

      p_io_buffer->input->mutex.unlock();

    }

    // отправляем ответ-квитирование, если он был сформирован в parse_input_data
    p_io_buffer->confirm->mutex.lock();
    write(p_io_buffer->confirm);
    p_io_buffer->confirm->mutex.unlock();

    // отправляем управляющие данные, если они есть
    p_io_buffer->output->mutex.lock();
    write(p_io_buffer->output);
    p_io_buffer->output->mutex.unlock();

  }
}


void SvRS::write(modus::BUFF* buffer)
{
  if(!buffer->ready())
    return;

  bool written = m_port->write(&buffer->data[0], buffer->offset) > 0;
  m_port->flush();

  if(written) {
    emit message(QString("<< %1").arg(QString(QByteArray((const char*)&buffer->data[0], buffer->offset).toHex())));
    buffer->reset();
  }
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
