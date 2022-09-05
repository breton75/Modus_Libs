#include "sv_can.h"

SvCAN::SvCAN():
  sock(0)
{

}

SvCAN::~SvCAN()
{
  close(sock);
}

bool SvCAN::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = CANParams::fromJsonString(p_config->interface.params);

    {
      // задаем парметры порта с помощью ip link
      QProcess p(this);
      QByteArray b;

      p.start(QString("sudo ip link set %1 down").arg(m_params.portname));

      if(p.waitForFinished(1000)) {

        b = p.readAllStandardError();

        if(!b.isEmpty())
          throw SvException(QString(ERR_PORT_ADJUST)
                            .arg(m_params.portname).arg(QString(b)));
      }
      else
        throw SvException(QString(ERR_PORT_ADJUST)
                          .arg(m_params.portname).arg(p.errorString()));

      p.start(QString("sudo ip link set %1 type can bitrate %2")
            .arg(m_params.portname)
            .arg(m_params.bitrate));

      if(p.waitForFinished(1000)) {

        b = p.readAllStandardError();

        if(!b.isEmpty())
          throw SvException(QString(ERR_PORT_ADJUST)
                            .arg(m_params.portname).arg(QString(b)));
      }
      else
        throw SvException(QString(ERR_PORT_ADJUST)
                          .arg(m_params.portname).arg(p.errorString()));

      p.start(QString("sudo ip link set %1 up").arg(m_params.portname));

      if(p.waitForFinished(1000)) {

        b = p.readAllStandardError();

        if(!b.isEmpty())
          throw SvException(QString(ERR_PORT_ADJUST)
                            .arg(m_params.portname).arg(QString(b)));

      }
    }

    if((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
      throw SvException("Ошибка при открытии порта");


    strcpy(ifr.ifr_name, m_params.portname.toStdString().c_str());
    ioctl(sock, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw SvException("Error in socket bind on init");



    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool SvCAN::start()
{
  p_is_active = true;

  QString err;
  int nbytes;
  int framesz = sizeof(can_frame);
//  qint64 reset_time = QDateTime::currentMSecsSinceEpoch() + p_config->interface.buffer_reset_interval;

  // по стандарту CAN между фреймами должен быть интервал длиной минимум 3 бита
  // вычисляем длину интервала в микросекундах!, исходя из заданного битрейта
  quint32 interval = int((1.0 / m_params.bitrate) * 1000000) * 3;

  qDebug() << interval;

  while(p_is_active) {

    memset(&frame, 0, framesz);

    nbytes = recv(sock, &frame, framesz, 0);

//    if(nbytes == 0) {

//      if(reset_time > QDateTime::currentMSecsSinceEpoch()) {

//        p_io_buffer->input->reset();
//        reset_time = QDateTime::currentMSecsSinceEpoch() + p_config->interface.buffer_reset_interval;

//      }
//    }
//    else
    if (nbytes < 0) {

      switch (errno) {

        case EAGAIN:
        case EINVAL:
          err = QString("Неверный файловый дескриптор 'sock'. Код ошибки %1").arg(errno);
          break;

        case EIO:
        case EINTR:
          err = QString("Ошибка ввода/вывода. Код ошибки %1").arg(errno);
          break;

        default:
          err = QString("Ошибка при чтении данных. Код ошибки %1").arg(errno);
          break;

      }

      message(QString("Устройство %1: %2").arg(p_config->name).arg(err));

    }

    else if (nbytes > 0) {

      if(nbytes != framesz) {

//        p_io_buffer->input->reset();

        emit message(QString("Неверный размер пакета. %1 байт вместо %2").arg(nbytes).arg(framesz),
                     sv::log::llError, sv::log::mtError);

        emit_message(QByteArray((const char*)&frame, nbytes), sv::log::llError, sv::log::mtError);


      }
      else {

        p_io_buffer->input->mutex.lock();

        if(p_io_buffer->input->offset + framesz > p_io_buffer->input->size)
          p_io_buffer->input->reset();

        memcpy(&p_io_buffer->input->data[p_io_buffer->input->offset], &frame, framesz);
        p_io_buffer->input->offset += framesz;

        emit_message(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], framesz), sv::log::llDebug, sv::log::mtReceive);

//        qDebug() << p_io_buffer->input->offset << QDateTime::currentDateTime().currentMSecsSinceEpoch() << QString(QByteArray(&((const char*)(&frame))[0], framesz).toHex());
        p_io_buffer->input->mutex.unlock();

      }
    }


    // отправляем управляющие данные, если они есть
    p_io_buffer->output->mutex.lock();

    if(p_io_buffer->output->isReady()) {

      int nbytes = send(sock, &p_io_buffer->output->data[0], sizeof(struct can_frame), 0);

      if(nbytes > 0) {

        emit_message(QByteArray((const char*)&p_io_buffer->output->data[0], p_io_buffer->output->offset),
            sv::log::llDebug, sv::log::mtSend);

        p_io_buffer->output->reset();
      }
    }

    p_io_buffer->output->mutex.unlock();

    usleep(interval);

  }
}

void SvCAN::emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type)
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
  modus::SvAbstractInterface* device = new SvCAN();
  return device;
}

const char* getDefaultParams()
{
  return "{ \"portname\": \"can0\", \"baudrate\": 19200, \"databits\": 8, \"parity\": 0, \"stopbits\": 2, \"flowcontrol\": 0 }";
}

const char* getName()
{
  return "Драйвер для работы с CAN шиной";
}

const char* getDescription()
{
  return "Драйвер для работы с CAN шиной";
}
