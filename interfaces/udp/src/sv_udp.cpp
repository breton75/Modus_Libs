#include "sv_udp.h"

SvUdp::SvUdp():
  m_socket(nullptr),
  m_forward_socket(nullptr)
{
}

SvUdp::~SvUdp()
{
  delete m_socket;
  delete m_forward_socket;

  deleteLater();
}

bool SvUdp::configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = udp::Params::fromJsonString(p_config->interface.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool SvUdp::start()
{
  try {

    m_socket = new QUdpSocket();

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


    connect(m_socket,     &QUdpSocket::readyRead,       this, &SvUdp::read);
    connect(p_io_buffer,  &modus::IOBuffer::readyWrite, this, &SvUdp::write);

    m_gap_timer = new QTimer;
    m_gap_timer->setInterval(m_params.grain_gap);
    m_gap_timer->setSingleShot(true);
    connect(m_gap_timer, &QTimer::timeout, this, &SvUdp::newData);

    if(m_params.forward_port)
      m_forward_socket = new QUdpSocket();

//    m_test_timer = new QTimer;
//    connect(m_test_timer, &QTimer::timeout, this, &SvUdp::newData);
//    m_test_timer->start(1000);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void SvUdp::read()
{
  m_gap_timer->stop();

//  if(p_io_buffer->input->isReady())
//    p_io_buffer->input->reset();

  p_io_buffer->input->mutex.lock();

  if(p_io_buffer->input->offset + m_socket->bytesAvailable() > p_config->bufsize)
    p_io_buffer->input->reset();

//    /* ... the rest of the datagram will be lost ... */
  qint64 readed = m_socket->readDatagram(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);

  if(p_io_buffer->input->offset == 0)
    p_io_buffer->input->set_time = QDateTime::currentMSecsSinceEpoch();

  emit_message(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed), sv::log::llDebug, sv::log::mtReceive);

  p_io_buffer->input->offset += readed;

  p_io_buffer->input->mutex.unlock();

  m_gap_timer->start(m_params.grain_gap);

}

void SvUdp::newData()
{
  QMutexLocker(&p_io_buffer->input->mutex);

//  p_io_buffer->input->setData(QByteArray::fromHex("01100aa00022441342 "
//                                                  "010000000000020000000000030000000000040000000000"
//                                                  "053920000000060000000000070000000000080000000000"
//                                                  "0900000000000A00000000000B0000000000218d"));

//  p_io_buffer->input->setData(QByteArray::fromHex(
//"0110041000050a020801008100010002000d60"));
  if(m_forward_socket) {

    qint64 w = m_forward_socket->writeDatagram(p_io_buffer->input->data, p_io_buffer->input->offset,
                                    m_params.forward_host, m_params.forward_port);\

    m_forward_socket->flush();

    emit message(QString("Forwarded %1 bytes to %2:%3")
                 .arg(w).arg(m_params.forward_host.toString()).arg(m_params.forward_port),
                 sv::log::llDebug, sv::log::mtData);

  }


  p_io_buffer->input->setReady(true);
  emit p_io_buffer->dataReaded(p_io_buffer->input);
}

void SvUdp::write(modus::BUFF* buffer)
{
  if(!buffer->isReady())
    return;

  buffer->mutex.lock();

  bool written = m_socket->writeDatagram(&buffer->data[0], buffer->offset, m_params.host, m_params.send_port) > 0;
  m_socket->flush();

  if(written)
    emit_message(QByteArray((const char*)&buffer->data[0], buffer->offset), sv::log::llDebug, sv::log::mtSend);

  buffer->reset();

  buffer->mutex.unlock();
}

void SvUdp::emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type)
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
  modus::SvAbstractInterface* device = new SvUdp();
  return device;
}
const char* getVersion()
{
  return LIB_VERSION;
}

const char* getParams()
{
  return udp::usage; //.toStdString().c_str(); // Params::usage().toStdString().c_str();
}

const char* getInfo()
{
  return LIB_SHORT_INFO;
}

const char* getDescription()
{
  return LIB_DESCRIPTION;
}
