#include "sv_iser.h"


iser::SvISER::SvISER(sv::SvAbstractLogger *logger):
  ad::SvAbstractDevice(logger)
{

}

iser::SvISER::~SvISER()
{
  emit stopThread();
  deleteLater();
}

bool iser::SvISER::configure(const ad::DeviceConfig &cfg)
{
  p_config = cfg;

  try {

    /* парсим - проверяем, что парметры устройства заданы верно */
    DeviceParams::fromJson(p_config.dev_params);

    /* парсим - проверяем, что парметры для указанного интерфейса заданы верно */
    switch (ifcesMap.value(p_config.ifc_name.toUpper(), AvailableIfces::Undefined)) {

      case AvailableIfces::UDP:

        UdpParams::fromJsonString(p_config.ifc_params);

        break;

//      case AvailableIfces::TEST:
//        TestParams::fromJsonString(p_config.ifc_params);
//        break;

      default:
        p_exception.raise(QString("Неизвестный тип интерфейса: %1").arg(p_config.ifc_name));
        break;
    }

    is_configured = true;

    return true;

  }

  catch(SvException& e) {

    is_configured = false;

    setLastError(e.error);
    return false;
  }
}

bool iser::SvISER::open()
{
  try {

    if(!is_configured)
      p_exception.raise(QString("Для устройства '%1' не задана конфигурация").arg(p_config.name));

    create_new_thread();

    connect(p_thread, &ad::SvAbstractDeviceThread::finished, this, &iser::SvISER::deleteThread);
    connect(this, &iser::SvISER::stopThread, p_thread, &ad::SvAbstractDeviceThread::stop);

    p_thread->open();
    p_thread->start();

    return true;

  } catch(SvException& e) {

    p_last_error = e.error;

    deleteThread();

    return false;

  }
}

void iser::SvISER::create_new_thread() throw(SvException)
{
  try {

    switch (ifcesMap.value(p_config.ifc_name.toUpper(), AvailableIfces::Undefined)) {

//      case AvailableIfces::RS485:
//      case AvailableIfces::RS:

//        p_thread = new iser::SerialThread(this, p_logger);

//        break;

      case AvailableIfces::UDP:

        p_thread = new iser::UDPThread(this, p_logger);
        break;


//      case AvailableIfces::TEST:
//        p_thread = new ConningKongsberTestThread(this, p_logger);
//        break;

    default:
      p_exception.raise(QString("Устройство %1. Неизвестный тип интерфейса: %2").arg(p_config.name).arg(p_config.ifc_name));
      break;

    }

    p_thread->conform(p_config.dev_params, p_config.ifc_params);

  }

  catch(SvException e) {
    throw e;

  }
}

void iser::SvISER::close()
{
  emit stopThread();

  p_isOpened = false;
}

void iser::SvISER::deleteThread()
{
  if(p_thread)
    delete p_thread;
}


/** ******* UDP THREAD ************* **/
iser::UDPThread::UDPThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  iser::GenericThread(device, logger)
{

}

iser::UDPThread::~UDPThread()
{
  deleteLater();
}

void iser::UDPThread::conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException)
{
  try {

    dev_params = DeviceParams::fromJson(jsonDevParams);
    ifc_params = UdpParams::fromJsonString(jsonIfcParams);

  }
  catch(SvException& e) {

    throw e;
  }
}

void iser::UDPThread::open() throw(SvException)
{
  if(!socket.bind(ifc_params.recv_port, QAbstractSocket::DontShareAddress))
    throw SvException(socket.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(dev_params.reset_interval);
  p_reset_timer.setSingleShot(true);

  connect(&socket, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &ad::SvAbstractDeviceThread::reset_buffer);

  // именно после open!
  socket.moveToThread(this);
}

void iser::UDPThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    process_signals();

    while(socket.waitForReadyRead(1000) && p_is_active) {

      while(socket.hasPendingDatagrams() && p_is_active)
      {
        if(socket.pendingDatagramSize() <= 0)
          continue;

        if(p_buff.offset > MAX_PACKET_SIZE)
          reset_buffer();

        if(!p_is_active)
          break;

        /* ... the rest of the datagram will be lost ... */
        p_buff.offset += socket.readDatagram((char*)(&p_buff.buf[p_buff.offset]), MAX_PACKET_SIZE - p_buff.offset);

        process_data();

      }
    }
  }

  socket.close();

}

quint64 iser::UDPThread::write(const QByteArray& data)
{
  if(!p_is_active)
    return 0;

  // небольшая задержка перед отправкой подтверждения
  // из-за того, что "шкаф не успевает обработать данные" (c) Гаврилов
  msleep(10);

  if(p_logger) // && p_device->info()->debug_mode)
    *p_logger //<< static_cast<dev::SvAbstractKsutsDevice*>(p_device)->make_dbus_sender()
              << sv::log::mtDebug
              << sv::log::llDebug
              << sv::log::TimeZZZ << sv::log::out
              << QString(data.toHex())
              << sv::log::endl;

//  QUdpSocket s;
//  quint64 w = s.writeDatagram(data, QHostAddress(p_ifc_params.host), p_ifc_params.send_port);
  quint64 w = socket.writeDatagram(data, ifc_params.host, ifc_params.send_port);
  socket.flush();

  return w;
}

void iser::UDPThread::stop()
{
  p_is_active = false;
}


/** **** GENERIC THREAD **** **/
iser::GenericThread::~GenericThread()
{

}

void iser::GenericThread::process_data()
{
  if(p_buff.offset >= m_hsz) {

    memcpy(&m_header, &p_buff.buf[0], m_hsz);

    if((m_header.sign != m_def_sign) ||
       ((dev_params.address != ISER_DEFAULT_ADDRESS) && (m_header.receiver_id != dev_params.address)) ||
       ((dev_params.sender_address != ISER_DEFAULT_SENDER_ADDRESS)  && (m_header.sender_id != dev_params.sender_address)))
    {
      reset_buffer();
      return;
    }

    if(p_buff.offset >= m_hsz + m_header.byte_count + 2) {

        if(p_logger) // && p_device->info()->debug_mode)
          *p_logger << me
                    << sv::log::mtDebug
                    << sv::log::llDebug
                    << sv::log::TimeZZZ << sv::log::in
                    << QString(QByteArray((const char*)&p_buff.buf[0], p_buff.offset).toHex())
                    << sv::log::endl;

        // если хоть какие то пакеты сыпятся (для данного получателя), то
        // считаем, что линия передачи в порядке и задаем новую контрольную точку времени
        p_device->setNewLostEpoch();

        // парсим и проверяем crc
        quint16 calc_crc = parse_data(&p_buff, &p_data, &m_header);

        if(calc_crc != p_data.crc) {

          // если crc не совпадает, то выходим без обработки и ответа
          if(p_logger)
              *p_logger << me
                        << sv::log::mtError
                        << sv::log::llError
                        << sv::log::TimeZZZ
                        << QString("Ошибка crc! Ожидалось %1, получено %2").arg(calc_crc, 0, 16).arg(p_data.crc, 0, 16)
                        << sv::log::endl;

        }
        else {

          quint64 len = 0;
          while(len < p_data.data_length) {

            QByteArray ba((const char*)(&p_data.data[0]), p_data.data_length);
            QDataStream stream_in(&ba, QIODevice::ReadOnly);

            QVariant v;
            stream_in >> v;

            qDebug() << v;

          }

        }

        reset_buffer();

    }
  }
}

quint16 iser::GenericThread::parse_data(ad::BUFF* buff, ad::DATA* data, iser::Header* header)
{
  size_t hSize = sizeof(iser::Header);

  // тип данных
  memcpy(&data->data_type, &buff->buf[0] + hSize, 1);

  // длина данных
  memcpy(&data->data_length, &buff->buf[0] + hSize + 1, 1);

  // данные
  memcpy(&data->data[0], &buff->buf[0] + hSize + 2, data->data_length);

  // crc полученная
  memcpy(&data->crc, &buff->buf[0] + hSize + header->byte_count, 2);

  // вычисляем crc из данных
  quint16 crc = CRC::MODBUS_CRC16(&buff->buf[0], hSize + header->byte_count);

  return crc;

}

QByteArray iser::GenericThread::confirmation()
{
  QByteArray confirm;
  confirm.append((const char*)(&m_header), 6);

  // вычисляем crc ответа
  quint16 crc = CRC::MODBUS_CRC16((uchar*)(&m_header), 6);
  confirm.append(quint8(crc & 0xFF));
  confirm.append(quint8(crc >> 8));

  return confirm;

}


/** ********** EXPORT ************ **/
ad::SvAbstractDevice* create()
{
  ad::SvAbstractDevice* device = new iser::SvISER();
  return device;
}


//VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc = QString())
//{
//  QString result = "";

//  switch(ifcesMap.value(ifc.toUpper()))
//  {
//    case RS:
//    case RS485:

//    result = QString("{\n"
//            "  \"ifc\": \"rs485\",\n"
//            "  \"portname\": \"ttyS0\",\n"
//            "  \"baudrate\": 19200,\n"
//            "  \"databits\": 8,\n"
//            "  \"flowcontrol\": 0,\n"
//            "  \"parity\": 0,\n"
//            "  \"stopbits\": 2\n"
//            "}");

//    break;


//  case UDP:

//    result = QString("{ \"host\": \"192.168.1.1\", \"recv_port\": 5300, \"send_port\": 5800 }");

//    break;
//  }

//  return result;

//}

//VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces()
//{
//  QList<QString> result = ifcesMap.keys();
//  return result;
//}

//VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams()
//{
//  return QString("{\n"
//                 "  \"start_register\": \"0x0000\",\n"
//                 "  \"reset_timeout\": 20\n"
//                 "}");
//}
