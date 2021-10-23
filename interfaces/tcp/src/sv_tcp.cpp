#include "sv_tcp.h"

SvTcp::SvTcp()
{
}

bool SvTcp::configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = TcpParams::fromJsonString(p_config->interface.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool SvTcp::start()
{
  try {

    m_socket = new QTcpSocket();

    if(!m_params.ifc.isEmpty()) {

      QNetworkInterface ifc = QNetworkInterface::interfaceFromName(m_params.ifc);
      if(!ifc.isValid())
        throw SvException(QString("Wrong ifc name: %1").arg(m_params.ifc));

      if(ifc.addressEntries().count() == 0)
        throw SvException(QString("Wrong ifc name: %1").arg(m_params.ifc));

      /* For TCP sockets, this function may be used to specify
       * which interface to use for an outgoing connection,
       * which is useful in case of multiple network interfaces */
    //  socket->bind(ifc.addressEntries().at(0).ip());

      if(!m_socket->bind(ifc.addressEntries().at(0).ip(), m_params.recv_port, QAbstractSocket::DontShareAddress))
        throw SvException(m_socket->errorString());
    }
    else
      if(!m_socket->bind(QHostAddress::Any, m_params.recv_port, QAbstractSocket::DontShareAddress))
        throw SvException(m_socket->errorString());


    connect(m_socket, &QTcpSocket::readyRead, this, &SvTcp::read);
    connect(p_io_buffer, &modus::IOBuffer::readyWrite, this, &SvTcp::write);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void SvTcp::read()
{
//  disconnect(m_socket, &QTcpSocket::readyRead, this, &SvTcp::read);
    p_io_buffer->input->mutex.lock();

    if(p_io_buffer->input->offset + m_socket->bytesAvailable() > p_config->bufsize)
      p_io_buffer->input->reset();

//    /* ... the rest of the datagram will be lost ... */
    qint64 readed = m_socket->readDatagram(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);
    emit message(QString(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed).toHex()), sv::log::llDebug, sv::log::mtReceive);

    p_io_buffer->input->offset = readed;

    while(m_socket->waitForReadyRead(m_params.grain_gap)) {

      while(m_socket->hasPendingDatagrams()) {
//        if(m_socket->pendingDatagramSize() <= 0)
//          continue;

        if(p_io_buffer->input->offset + m_socket->bytesAvailable() > p_config->bufsize)
          p_io_buffer->input->reset();

        /* ... the rest of the datagram will be lost ... */
        qint64 readed2 = m_socket->readDatagram(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);

//qDebug() << QString(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed).toHex());
        emit message(QString(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed2).toHex()), sv::log::llDebug, sv::log::mtReceive);
        p_io_buffer->input->offset += readed2;

      }
    }

    p_io_buffer->input->mutex.unlock();

    emit p_io_buffer->dataReaded(p_io_buffer->input);

}


void SvTcp::write(modus::BUFF* buffer)
{
  QMutexLocker(&(buffer->mutex));

  if(!buffer->ready())
    return;

  bool written = m_socket->writeDatagram(&buffer->data[0], buffer->offset, m_params.host, m_params.send_port) > 0;
  m_socket->flush();

  if(written)
    emit message(QString(QByteArray((const char*)&buffer->data[0], buffer->offset).toHex()), sv::log::llDebug, sv::log::mtSend);

  buffer->reset();

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
