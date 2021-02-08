#include "proj_12700_opa_mod.h"

opa::SvOPA::SvOPA():
  modus::SvAbstractProtocol()
{
  signal_collections.insert(TYPE_0x33, &type0x33_signals);
  signal_collections.insert(TYPE_0x02, &type0x02_signals);
  signal_collections.insert(TYPE_0x03, &type0x03_signals);
  signal_collections.insert(TYPE_0x04, &type0x04_signals);
  signal_collections.insert(TYPE_0x19, &type0x19_signals);
}

bool opa::SvOPA::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = opa::DeviceParams::fromJson(p_config->protocol.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void opa::SvOPA::disposeInputSignal (modus::SvSignal* signal)
{
  try {

    QString type = signal->config()->type.toLower();

    if(type == "status")
      line_status_signals.addSignal(signal);

    else {

      bool ok;
      int itype = type.toInt(&ok, 0);

      if(ok && signal_collections.contains(itype))
        signal_collections.value(itype)->addSignal(signal);

      else {

        emit message(QString("Сигнал %1: Неопознанный тип \"%2\"").arg(signal->config()->name).arg(signal->config()->type),
                     sv::log::llError, sv::log::mtError);

      }
    }
  }

  catch(SvException& e) {
    throw e;
  }
}

void opa::SvOPA::disposeOutputSignal (modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void opa::SvOPA::run()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);

  while(p_is_active) {

    p_io_buffer->confirm->mutex.lock();
    p_io_buffer->input->mutex.lock();     // если нужен ответ квитирование

    opa::PARSERESULT result = parse();

    if(result.do_reset == DO_RESET)
      p_io_buffer->input->reset();

    p_io_buffer->input->mutex.unlock();     // если нужен ответ квитирование
    p_io_buffer->confirm->mutex.unlock();

    if(result.parse_time.isValid())
      validateSignals(result.parse_time);

  }
}

opa::PARSERESULT opa::SvOPA::parse()
{
  // проверяем, что длина данных в буфере не меньше длины звголовка
  if(p_io_buffer->input->offset < m_hsz)
    return opa::PARSERESULT(DO_NOT_RESET);

  // разбираем заголовок. если адрес или код функции не тот, значит это чужой пакет
  memcpy(&m_header, &p_io_buffer->input->data[0], m_hsz);
  if((m_header.client_addr != 1) || (m_header.func_code != 0x10))
    return opa::PARSERESULT(DO_RESET);

  // проверяем, что длина данных в буфере не меньше длины всего отправленного пакета
  if(p_io_buffer->input->offset < m_hsz + m_header.byte_count + 2)
    return opa::PARSERESULT(DO_NOT_RESET);

  // проверяем начальный и конечный адреса регистров. если не входит в заданный диапазон - чужой пакет
  quint16 current_register = (static_cast<quint16>(m_header.ADDRESS << 8)) + m_header.OFFSET;

  if((current_register < m_params.start_register) ||
     (current_register > m_params.last_register))
    return opa::PARSERESULT(DO_RESET);

  /**
  *  в этой точке в буфере должны находиться правильные данные
  *  производим непосредственно разбор данных и назначаем значения сигналам
  **/
  emit message(QString(">> %1").arg(QString(QByteArray((const char*)&p_io_buffer->input->data[0], p_io_buffer->input->offset).toHex())));

  // если хоть какие то пакеты сыпятся (для данного получателя), то
  // считаем, что линия передачи в порядке и задаем новую контрольную точку времени
//  p_device->setNewLostEpoch();

  // ставим состояние данной линии
  line_status_signals.updateSignals();

  // парсим и проверяем crc
  memcpy(&m_data.data_type,   &p_io_buffer->input->data[0] + m_hsz, 1);                       // тип данных
  memcpy(&m_data.data_length, &p_io_buffer->input->data[0] + m_hsz + 1, 1);                   // длина данных
  memcpy(&m_data.data[0],     &p_io_buffer->input->data[0] + m_hsz + 2, m_data.data_length);  // данные
  memcpy(&m_data.crc,         &p_io_buffer->input->data[0] + m_hsz + m_header.byte_count, 2); // crc полученная

  quint16 calc_crc = CRC::MODBUS_CRC16((const quint8*)&p_io_buffer->input->data[0], m_hsz + m_header.byte_count); // вычисляем crc из данных

  if(calc_crc != m_data.crc) {

    // если crc не совпадает, то выходим без обработки и ответа
    emit message(QString("Ошибка crc! Ожидалось %1%2, получено %3%4")
                 .arg(quint8(calc_crc), 2, 16, QChar('0'))
                 .arg(quint8(calc_crc >> 8), 2, 16, QChar('0'))
                 .arg(quint8(m_data.crc), 2, 16, QChar('0'))
                 .arg(quint8(m_data.crc >> 8), 2, 16, QChar('0')),
                 sv::log::llError, sv::log::mtError);

    return opa::PARSERESULT(DO_RESET);
  }

  // если все корректно, то разбираем данные в зависимости от типа
  switch (current_register - m_params.start_register)
  {
      case 0x00:
      case 0x03:
      case 0x05:

        // здесь просто отправляем ответ-квитирование
        confirmation();

        if(m_data.data_type == 0x77) {

          for (modus::SvSignal* signal: p_input_signals)
            signal->setValue(0);
        }

        break;

      case 0x06:
      case 0x10:
      case 0x50:
      case 0x90:
      {
         // формируем и отправляем ответ-квитирование
         confirmation();

         if(signal_collections.contains(m_data.data_type))
           signal_collections.value(m_data.data_type)->updateSignals(&m_data);

         break;
      }

      default:
          break;
  }

  return opa::PARSERESULT(DO_RESET, QDateTime::currentDateTime());
}

void opa::SvOPA::confirmation()
{
  QByteArray confirm;
  confirm.append((const char*)(&m_header), 6);

  // вычисляем crc ответа
  quint16 crc = CRC::MODBUS_CRC16((uchar*)(&m_header), 6);
  confirm.append(quint8(crc & 0xFF));
  confirm.append(quint8(crc >> 8));

  memcpy(&p_io_buffer->confirm->data[0], confirm.data(), confirm.length());
  p_io_buffer->confirm->offset = confirm.length();

}


/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* device = new opa::SvOPA();
  return device;
}
