#include "sv_udp.h"

SvUdp::SvUdp()
{
}

bool SvUdp::configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = UdpParams::fromJsonString(p_config->interface.params);

    m_socket = new QUdpSocket();

    if(!m_socket->bind(QHostAddress::Any, m_params.recv_port, QAbstractSocket::DontShareAddress))
      throw SvException(m_socket->errorString());

    // именно после всего!
    m_socket->moveToThread(this);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void SvUdp::run()
{
  p_is_active = true;

  while(p_is_active) {

    while(m_socket->waitForReadyRead(m_params.buffer_reset_interval) && p_is_active) {

      while(m_socket->hasPendingDatagrams() && p_is_active) {

        if(m_socket->pendingDatagramSize() <= 0)
          continue;

        if(p_io_buffer->input->offset > p_config->bufsize)
          p_io_buffer->input->reset();

        p_io_buffer->input->mutex.lock();
        /* ... the rest of the datagram will be lost ... */
        qint64 readed = m_socket->readDatagram((char*)(&p_io_buffer->input->data[p_io_buffer->input->offset]), p_config->bufsize - p_io_buffer->input->offset);

        emit message(QString(">> %1").arg(QString(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed).toHex())), sv::log::llDebug, sv::log::mtReceive);

        p_io_buffer->input->offset += readed;

        p_io_buffer->input->mutex.unlock();

        // переключаемся на другой поток
        QThread::yieldCurrentThread();

        p_io_buffer->confirm->mutex.lock();
        if(p_io_buffer->confirm->ready()) {

          write(p_io_buffer->confirm);
        }
        p_io_buffer->confirm->mutex.unlock();

      }
    }

    p_io_buffer->input->mutex.lock();
    p_io_buffer->input->reset();
    p_io_buffer->input->mutex.unlock();

    // отправляем управляющие данные, если они есть
    p_io_buffer->output->mutex.lock();
//    qDebug() << "write" << int(p_io_buffer->output->data[256]);
    write(p_io_buffer->output);
    p_io_buffer->output->mutex.unlock();

  }
}


void SvUdp::write(modus::BUFF* buffer)
{
  if(!buffer->ready())
    return;

  bool written = m_socket->writeDatagram(&buffer->data[0], buffer->offset, m_params.host, m_params.send_port) > 0;
  m_socket->flush();

  if(written)
    emit message(QString("<< %1").arg(QString(QByteArray((const char*)&buffer->data[0], buffer->offset).toHex())), sv::log::llDebug, sv::log::mtSend);

  buffer->reset();

}


/** ********** EXPORT ************ **/
modus::SvAbstractInterface* create()
{
  modus::SvAbstractInterface* device = new SvUdp();
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
