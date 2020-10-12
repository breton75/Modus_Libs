#include "sv_opa.h"


opa::SvOPA::SvOPA(sv::SvAbstractLogger *logger):
  ad::SvAbstractDevice(logger)
{
//  SignalsGEN.clear();
//  SignalsXDR.clear();
}

opa::SvOPA::~SvOPA()
{
  emit stopThread();
  deleteLater();
}

bool opa::SvOPA::configure(const ad::DeviceConfig &cfg)
{
  p_config = cfg;

  try {

    /* парсим - проверяем, что парметры устройства заданы верно */
    DeviceParams::fromJson(p_config.dev_params);

    /* парсим - проверяем, что парметры для указанного интерфейса заданы верно */
    switch (ifcesMap.value(p_config.ifc_name.toUpper(), AvailableIfces::Undefined)) {

      case AvailableIfces::RS485:
      case AvailableIfces::RS:

        SerialParams::fromJsonString(p_config.ifc_params);

        break;

      case AvailableIfces::UDP:

        UdpParams::fromJsonString(p_config.ifc_params);

        break;

      case AvailableIfces::TEST:
        TestParams::fromJsonString(p_config.ifc_params);
        break;

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

void opa::SvOPA::addSignal(SvSignal* signal) throw(SvException)
{
  ad::SvAbstractDevice::addSignal(signal);

  ckng::SignalParams p = ckng::SignalParams::fromSignal(signal);

  if(signal->config()->type == "GEN")
    SignalsGEN.insert(p.group * 16 + p.word, signal->config()->name);

  else if(signal->config()->type == "XDR")
    SignalsXDR.insert(p.group * 16 + p.word, signal->config()->name);

}

bool opa::SvOPA::open()
{
  try {

    if(!is_configured)
      p_exception.raise(QString("Для устройства '%1' не задана конфигурация").arg(p_config.name));

    create_new_thread();

    p_thread->conform(p_config.dev_params, p_config.ifc_params);

    connect(p_thread, &ad::SvAbstractDeviceThread::finished, this, &opa::SvOPA::deleteThread);
    connect(this, &opa::SvOPA::stopThread, p_thread, &ad::SvAbstractDeviceThread::stop);

    p_thread->open();
    p_thread->start();

    return true;

  } catch(SvException& e) {

    p_last_error = e.error;
//    p_log << sv::log::mtError << sv::log::llError << e.error << sv::log::endl;

    deleteThread();

    return false;

  }
}

void opa::SvOPA::create_new_thread() throw(SvException)
{
  try {

    switch (ifcesMap.value(p_config.ifc_name.toUpper(), AvailableIfces::Undefined)) {

      case AvailableIfces::RS485:
      case AvailableIfces::RS:

        p_thread = new opa::SerialThread(this, p_logger);

        break;

      case AvailableIfces::UDP:

        p_thread = new opa::UDPThread(this, p_logger);
        break;


//      case AvailableIfces::TEST:
//        p_thread = new ConningKongsberTestThread(this, p_logger);
//        break;

    default:
      p_exception.raise(QString("Неизвестный тип интерфейса: %1").arg(config()->ifc_name));
      break;

    }

//    static_cast<opa::GenericThread*>(p_thread)->setSignalsMap(&SignalsGEN, &SignalsXDR);

  }

  catch(SvException& e) {

    throw e;

  }
}

void opa::SvOPA::close()
{
  emit stopThread();

  p_isOpened = false;
}

void opa::SvOPA::deleteThread()
{
  if(p_thread)
    delete p_thread;
}


/** ******* UDP THREAD ************* **/
opa::UDPThread::UDPThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  opa::GenericThread(device, logger)
{

}

opa::UDPThread::~UDPThread()
{
  deleteLater();
}

void opa::UDPThread::conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException)
{
  try {

    dev_params = DeviceParams::fromJson(jsonDevParams);
    ifc_params = UdpParams::fromJsonString(jsonIfcParams);

    foreach (SvSignal* signal, p_device->Signals()->values()) {



    }

  }
  catch(SvException& e) {

    throw e;
  }
}

void opa::UDPThread::open() throw(SvException)
{
  if(!socket.bind(QHostAddress::Any, ifc_params.recv_port, QAbstractSocket::DontShareAddress))
    throw SvException(socket.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(dev_params.reset_timeout);
  p_reset_timer.setSingleShot(true);

  connect(&socket, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &ad::SvAbstractDeviceThread::reset_buffer);

  // именно после open!
  socket.moveToThread(this);
}

void opa::UDPThread::run()
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

quint64 opa::UDPThread::write(const QByteArray& data)
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

void opa::UDPThread::stop()
{
  p_is_active = false;
}


/** ******* Serial THREAD ************* **/
opa::SerialThread::SerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger):
  opa::GenericThread(device, logger)
{

}

opa::SerialThread::~SerialThread()
{
  deleteLater();
}

void opa::SerialThread::conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException)
{
  try {

    dev_params = DeviceParams::fromJson(jsonDevParams);
    ifc_params = SerialParams::fromJsonString(jsonIfcParams);

  }
  catch(SvException& e) {

    throw e;
  }
}

void opa::SerialThread::open() throw(SvException)
{
  port.setPortName   (ifc_params.portname   );
  port.setBaudRate   (ifc_params.baudrate   );
  port.setStopBits   (ifc_params.stopbits   );
  port.setFlowControl(ifc_params.flowcontrol);
  port.setDataBits   (ifc_params.databits   );
  port.setParity     (ifc_params.parity     );

  if(!port.open(QIODevice::ReadWrite))
    throw p_exception.assign(port.errorString());

  // с заданным интервалом сбрасываем буфер, чтобы отсекать мусор и битые пакеты
  p_reset_timer.setInterval(dev_params.reset_timeout);

  connect(&port, SIGNAL(readyRead()), &p_reset_timer, SLOT(start()));
  connect(&p_reset_timer, &QTimer::timeout, this, &ad::SvAbstractDeviceThread::reset_buffer);

  // именно после open!
  port.moveToThread(this);

}

void opa::SerialThread::run()
{
  p_is_active = true;

  while(p_is_active) {

    process_signals();

    while(port.waitForReadyRead(1000) && p_is_active) {

      if(p_buff.offset > MAX_PACKET_SIZE)
        reset_buffer();

      p_buff.offset += port.read((char*)(&p_buff.buf[p_buff.offset]), MAX_PACKET_SIZE - p_buff.offset);

      process_data();

    }
  }

  port.close();

}

quint64 opa::SerialThread::write(const QByteArray& data)
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

  return port.write(data);

}

void opa::SerialThread::stop()
{
  p_is_active = false;
}


/** ******* Test THREAD ************* **/

//ConningKongsberTestThread::ConningKongsberTestThread(ad::SvAbstractDevice *device, sv::SvAbstractLogger* logger):
//  opa::GenericThread(device, logger)
//{

//}

//ConningKongsberTestThread::~ConningKongsberTestThread()
//{
//  deleteLater();
//}

//void ConningKongsberTestThread::conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException)
//{
//  try {

//    dev_params = DeviceParams::fromJson(jsonDevParams);
//    ifc_params = TestParams::fromJsonString(jsonIfcParams);

//  }
//  catch(SvException& e) {
//    throw e;
//  }
//}

//void ConningKongsberTestThread::open() throw(SvException)
//{
//  *p_logger << sv::log::mtSuccess << QString("%1 открыт").arg(p_device->config()->name) << sv::log::endl;
//}

//quint64 ConningKongsberTestThread::write(const QByteArray& data)
//{
//  Q_UNUSED(data);

//  *p_logger << sv::log::mtSuccess << QString("%1 записал данные").arg(p_device->config()->name) << sv::log::endl;

//  return 0;
//}

//void ConningKongsberTestThread::run()
//{
//  p_is_active = true;

//  int ref = 1;
//  while(p_is_active)
//  {

//    process_signals();

//    QString msg = QString("$IIXDR,%1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,1,0,7*22dd").arg(ref);

//    memcpy(&p_buff.buf[0], msg.toStdString().c_str(), msg.length());
//    p_buff.offset = msg.length();

//    process_data();

////    if(ifc_params.show_time)
////      *p_logger << sv::log::mtInfo << sv::log::TimeZZZ << ifc_params.testmsg << ifc_params.testval << sv::log::endl;
////    else
////      *p_logger << sv::log::mtInfo << ifc_params.testmsg << ifc_params.testval << sv::log::endl;

//    msleep(ifc_params.period);
//  }
//}

//void ConningKongsberTestThread::stop()
//{
//  p_is_active = false;
//}


/** **** GENERIC FUNCTIONS **** **/
//void opa::GenericThread::setSignalsMap(ckng::SignalsMap *smapGEN, ckng::SignalsMap *smapXDR)
//{
//  SignalsGEN = smapGEN;
//  SignalsXDR = smapXDR;

////  signals_by_reference = smap;
////  return sv::log::sender::make(p_logger->options().log_sender_name_format,
////                               p_info.name,
////                               p_info.index);
//}

//void opa::GenericThread::process_signals()
//{
//  foreach (SvSignal* signal, p_device->Signals()->values()) {
//    if((signal->config()->timeout > 0 && !signal->isAlive()) ||
//       (signal->config()->timeout == 0 && !signal->isDeviceAlive()))
//          signal->setLostValue();
//  }
//}

void opa::GenericThread::process_data()
{
  if(p_buff.offset >= m_hsz) {

    memcpy(&m_header, &p_buff.buf[0], m_hsz);

    if((m_header.client_addr != 1) || (m_header.func_code != 0x10)) {

      reset_buffer();
      return;
    }

    if(p_buff.offset >= m_hsz + m_header.byte_count + 2) {

        quint16 current_register = (static_cast<quint16>(m_header.ADDRESS << 8)) + m_header.OFFSET;

        if((current_register < dev_params.start_register) ||
           (current_register > dev_params.last_register))
        {
           reset_buffer();
           return;
        }


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

        // ставим состояние данной линии
        func_set_line_status(p_device, &p_data);

        switch (current_register - dev_params.start_register)
        {
            case 0x00:
            case 0x03:
            case 0x05:

              // здесь просто отправляем ответ-квитирование
              write(confirmation());

              if(p_data.data_type == 0x77)
                func_0x77(p_device);

              break;

            case 0x06:
            case 0x10:
            case 0x50:
            case 0x90:
            {
              // парсим и проверяем crc
              quint16 calc_crc = parse_data(&p_buff, &p_data, &_header);


              if(calc_crc != p_data.crc)
              {
                // если crc не совпадает, то выходим без обработки и ответа
                if(p_logger)
                    *p_logger << me
                              << sv::log::mtError
                              << sv::log::llError
                              << sv::log::TimeZZZ
                              << QString("Ошибка crc! Ожидалось %1, получено %2").arg(calc_crc, 0, 16).arg(p_data.crc, 0, 16)
                              << sv::log::endl;

              }
              else
              {
                  // формируем и отправляем ответ-квитирование
                  write(confirmation());

                  // раскидываем данные по сигналам, в зависимости от типа данных
                  switch (p_data.data_type) {

                    case 0x19: opa::func_0x19(p_device, &p_data); break;
                    case 0x02: opa::func_0x02(p_device, &p_data); break;
                    case 0x03: opa::func_0x03(p_device, &p_data); break;
                    case 0x04: opa::func_0x04(p_device, &p_data); break;

                  }
                }

              break;
            }

            default:
                break;
        }

        reset_buffer();

    }
  }

}

quint16 opa::GenericThread::parse_data(ad::BUFF* buff, ad::DATA* data, opa::Header* header)
{
  size_t hSize = sizeof(opa::Header);

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

QByteArray opa::GenericThread::confirmation()
{
  QByteArray confirm;
  confirm.append((const char*)(&m_header), 6);

  // вычисляем crc ответа
  quint16 crc = CRC::MODBUS_CRC16((uchar*)(&m_header), 6);
  confirm.append(quint8(crc & 0xFF));
  confirm.append(quint8(crc >> 8));

  return confirm;

}

/** в результате несогласованности, получилось 2 набора сигналов состояния линии **/
void opa::GenericThread::func_set_line_status(ad::SvAbstractDevice* device, ad::DATA* data)
{
  Q_UNUSED(data);

  if(reg2STATUS.contains(dev_params.start_register))
    device->setSignalValue(reg2STATUS.value(dev_params.start_register), 1);
}

void opa::GenericThread::func_0x77(ad::SvAbstractDevice* device)
{
  foreach (SvSignal* signal, device->Signals()->values())
    signal->setValue(0);
}

void opa::GenericThread::func_0x19(ad::SvAbstractDevice* device, ad::DATA* data)
{
  if(reg2FI.contains(dev_params.start_register))
    device->setSignalValue(reg2FI.value(dev_params.start_register), data->data[0]);
}

void opa::GenericThread::func_0x02(ad::SvAbstractDevice* device, ad::DATA* data)
{
  quint8 data_begin = 0;
  quint16 detector_num;
  quint8  factor;
  QString signal_name;

  while(data_begin < data->data_length) {

    memcpy(&detector_num, &data->data[data_begin], sizeof(quint16));
    memcpy(&factor, &data->data[data_begin + sizeof(quint16)], sizeof(quint8));

    if(factor & 0x7F) {

      if(SIGNALS_TABLE.contains(factor)) {

        signals_by_detector* sbd = SIGNALS_TABLE.value(factor);

        if(sbd->contains(detector_num)) {

          signal_name = sbd->value(detector_num);
//          qDebug() << QString("detector_num: %1   factor: %2  signal_name: %3").arg(detector_num).arg(factor).arg(signal_name);

          QString prefix = opa::GenericThread::getPrefix(dev_params.start_register);
          signal_name.push_front(prefix);

          device->setSignalValue(signal_name, 1.0);

        }
      }


        /* здесь применяем следующее решение
         * есть датчики, у которых только один порог срабатывания. НО выяснилось, что
         * у некоторых (или у всех) это ВТОРОЙ порог. соответственно, те сигналы,
         * которые сидят в таблицах для первого порога, должны сидеть в таблицах
         * второго. поэтому, если пришла сработка по второму порогу, ищем нужный сигнал
         * в таблицах второго порога. если там его нет, то пытаемся найти его в таблицах
         * первого. если находим, то фиксируем сработку */

        // если сработка по 2ому порогу, и не нашли в таблицах 2го порога, то ищем в таблицах для 1го порога
        else if (factor & 0x80) {


          factor &= 0x7F;
          if(SIGNALS_TABLE.contains(factor)) {

            signals_by_detector* sbd = SIGNALS_TABLE.value(factor);

            if(sbd->contains(detector_num)) {

              signal_name = sbd->value(detector_num);
      //        qDebug() << QString("detector_num: %1   factor: %2  signal_name: %3").arg(detector_num).arg(factor).arg(signal_name);

              QString prefix = opa::getPrefix(dev_params.start_register);
              signal_name.push_front(prefix);

              device->setSignalValue(signal_name, 1.0);

            }
          }
      }

      /* теперь все то же самое делаем для дублирующих сигналов Z */
      if(SIGNALS_Z_TABLE.contains(factor)) {

        signals_by_detector* sbd = SIGNALS_Z_TABLE.value(factor);

        if(sbd->contains(detector_num)) {

          signal_name = sbd->value(detector_num);
//          qDebug() << QString("detector_num: %1   factor: %2  signal_name: %3").arg(detector_num).arg(factor).arg(signal_name);

          QString prefix = opa::getPrefix(dev_params.start_register);
          signal_name.push_front(prefix);

          device->setSignalValue(signal_name, 1.0);

        }

        // если сработка по 2ому порогу, и не нашли в таблицах 2го порога, то ищем в таблицах для 1го порога
        else if (factor & 0x80) {

          factor &= 0x7F;

          if(SIGNALS_Z_TABLE.contains(factor)) {

            signals_by_detector* sbd = SIGNALS_Z_TABLE.value(factor);

            if(sbd->contains(detector_num)) {

              signal_name = sbd->value(detector_num);
      //        qDebug() << QString("detector_num: %1   factor: %2  signal_name: %3").arg(detector_num).arg(factor).arg(signal_name);

              QString prefix = opa::getPrefix(dev_params.start_register);
              signal_name.push_front(prefix);

              device->setSignalValue(signal_name, 1.0);

            }
          }
        }
      }

    }

    else {

      foreach (signals_by_detector* sbd, SIGNALS_TABLE.values()) {

        foreach (quint16 dn, sbd->keys()) {

          if(dn == detector_num) {

            signal_name = sbd->value(dn);

            QString prefix = opa::getPrefix(dev_params.start_register);
            signal_name.push_front(prefix);

            device->setSignalValue(signal_name, 0.0);

          }
        }
      }

      foreach (signals_by_detector* sbd, SIGNALS_Z_TABLE.values()) {

        foreach (quint16 dn, sbd->keys()) {

          if(dn == detector_num) {

            signal_name = sbd->value(dn);

            QString prefix = opa::getPrefix(dev_params.start_register);
            signal_name.push_front(prefix);

            device->setSignalValue(signal_name, 0.0);

          }
        }
      }
    }

    data_begin += 4; // 4 байта данных на один извещатель

  }
}

void opa::GenericThread::func_0x03(ad::SvAbstractDevice* device, ad::DATA* data)
{
  quint8 data_begin = 0;
  quint16 room_num;
  quint8  level;
  QString signal_name;

  while(data_begin < data->data_length) {

    memcpy(&room_num, &data->data[data_begin], sizeof(quint16));
    memcpy(&level, &data->data[data_begin + sizeof(quint16)], sizeof(quint8));

    if(SIGNALS_BY_ROOMS.contains(room_num)) {

      signal_name = SIGNALS_BY_ROOMS.value(room_num);

      QString prefix = opa::getPrefix(dev_params.start_register);
      signal_name.push_front(prefix);

      device->setSignalValue(signal_name, static_cast<qreal>(level));

//      if(config()->debug)
//        qDebug() << QString("sig: %1  room: %2  lvl: %3").arg(signal_name).arg(room_num).arg(level);

    }


    if(SIGNALS_Z_BY_ROOMS.contains(room_num)) {

      signal_name = SIGNALS_Z_BY_ROOMS.value(room_num);

      QString prefix = opa::getPrefix(dev_params.start_register);
      signal_name.push_front(prefix);

      device->setSignalValue(signal_name, static_cast<qreal>(level));

//      if(config()->debug)
//        qDebug() << QString("sig: %1  room: %2  lvl: %3").arg(signal_name).arg(room_num).arg(level);

    }

    data_begin += 4; // 4 байта данных на один извещатель

  }
}

void opa::GenericThread::func_0x04(ad::SvAbstractDevice* device, ad::DATA* data)
{
  QString prefix = opa::getPrefix(dev_params.start_register);

  device->setSignalValue(QString("%1%2").arg(prefix).arg(BI25_5SS1_VD1), qreal(CALC_BI25_5SS1_VD1( data->data[0] ) ));
  device->setSignalValue(QString("%1%2").arg(prefix).arg(BI25_5SS1_VD2), qreal(CALC_BI25_5SS1_VD2( data->data[0] ) ));
  device->setSignalValue(QString("%1%2").arg(prefix).arg(BI26_6SS1_VD1), qreal(CALC_BI26_6SS1_VD1( data->data[0] ) ));
}

inline QString opa::getPrefix(quint16 start_register)
{
  QString prefix = "";

  switch (start_register) {

    case 0x04A0:
      prefix = "C122_";
      break;

    case 0x0540:
      prefix = "C8_";

    case 0x0680:
      prefix = "C38_";

    case 0x0720:
      prefix = "C67_";

    case 0x07C0:
      prefix = "C93_";

    case 0x0860:
      prefix = "C123_";

    case 0x0900:
      prefix = "C150_";

  }

  return prefix;
}

/** ********** EXPORT ************ **/
ad::SvAbstractDevice* create()
{
  ad::SvAbstractDevice* device = new opa::SvOPA();
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
