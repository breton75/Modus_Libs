﻿#include "apak_raduga_sim.h"

raduga::SvRaduga::SvRaduga():
  modus::SvAbstractProtocol()
{
  input_signal_collections.insert(TYPE_1,   &type1_input_signals);
  input_signal_collections.insert(TYPE_2,   &type2_input_signals);
  input_signal_collections.insert(TYPE_3,   &type3_input_signals);
  input_signal_collections.insert(TYPE_5,   &type5_input_signals);
  input_signal_collections.insert(TYPE_9,   &type9_input_signals);
  input_signal_collections.insert(TYPE_53,  &type53_input_signals);

  output_signal_collections.insert(TYPE_1,  &type1_output_signals);
  output_signal_collections.insert(TYPE_2,  &type2_output_signals);
  output_signal_collections.insert(TYPE_3,  &type3_output_signals);
  output_signal_collections.insert(TYPE_5,  &type5_output_signals);
  output_signal_collections.insert(TYPE_9,  &type9_output_signals);
  output_signal_collections.insert(TYPE_53, &type53_output_signals);

}

bool raduga::SvRaduga::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

//    m_params = raduga::ProtocolParams::fromJson(p_config->protocol.params);

    if(!m_data.resize(p_config->bufsize))
      throw SvException(QString("Не удалось выделить %1 байт памяти для буфера").arg(p_config->bufsize));

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void raduga::SvRaduga::disposeInputSignal (modus::SvSignal* signal)
{
  try {

    QString type = signal->config()->packid;

    bool ok;
    quint16 itype = type.toUInt(&ok, 0);

    if(ok && input_signal_collections.contains(itype))
      input_signal_collections.value(itype)->addSignal(signal);

    else {

      emit message(QString("Сигнал %1: Неопознанный тип \"%2\"").arg(signal->config()->name).arg(signal->config()->type),
                   sv::log::llError, sv::log::mtError);

    }
  }

  catch(SvException& e) {
    throw e;
  }
}

void raduga::SvRaduga::disposeOutputSignal (modus::SvSignal* signal)
{
  try {

    QString pack_id = signal->config()->packid;

    bool ok;
    quint16 ipack = pack_id.toUInt(&ok, 0);

    if(ok && output_signal_collections.contains(ipack))
      output_signal_collections.value(itype)->addSignal(signal);

    else {

      emit message(QString("Сигнал %1: Неопознанный тип \"%2\"").arg(signal->config()->name).arg(signal->config()->type),
                   sv::log::llError, sv::log::mtError);

    }
  }

  catch(SvException& e) {
    throw e;
  }
}

void raduga::SvRaduga::run()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);

  while(p_is_active) {

//    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

    raduga::TREATRESULT result = parse();

    if(result.do_reset == DO_RESET)
      p_io_buffer->input->reset();

    p_io_buffer->input->mutex.unlock();
//    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

    if(result.parse_time.isValid())
      validateSignals(result.parse_time);

    p_io_buffer->output->mutex.lock();

    putout();

    p_io_buffer->output->mutex.unlock();

    QThread::yieldCurrentThread();

    msleep(1);

  }
}

raduga::TREATRESULT raduga::SvRaduga::parse()
{
  // проверяем, что длина данных в буфере не меньше длины заголовка
  if(p_io_buffer->input->offset < m_hsz)
    return raduga::TREATRESULT(DO_NOT_RESET);

  // разбираем заголовок. если адрес или код функции не тот, значит это чужой пакет
  memcpy(&m_header, &p_io_buffer->input->data[0], m_hsz);
  if((m_header.abonent_id != m_params.abonent) || (m_header.pack_id != m_params.packid))
    return raduga::TREATRESULT(DO_RESET);

  /**
  *  в этой точке в буфере должны находиться правильные данные
  *  производим непосредственно разбор данных и назначаем значения сигналам
  **/
//  qDebug() << QString(QByteArray((const char*)&p_io_buffer->input->data[0], p_io_buffer->input->offset).toHex());
  message(QString(">> %1").arg(QString(QByteArray((const char*)&p_io_buffer->input->data[0], p_io_buffer->input->offset).toHex())));

  // если хоть какие то пакеты сыпятся (для данного получателя), то
  // считаем, что линия передачи в порядке и задаем новую контрольную точку времени
//  p_device->setNewLostEpoch();

  // ставим состояние данной линии
//  line_status_signals.updateSignals();

//  // парсим и проверяем crc
//  memcpy(&m_data.type, &p_io_buffer->input->data[m_hsz], 1);                       // тип данных
//  memcpy(&m_data.len,  &p_io_buffer->input->data[m_hsz + 1], 1);                   // длина данных
//  memcpy(&m_data.crc,  &p_io_buffer->input->data[m_hsz + m_header.byte_count], 2); // crc полученная

//  // проверяем, что длина данных не превышает размер выделенного буфера
//  if(m_data.len > m_data.bufsize) {

//    message(QString("Размер данных превышает размер буфера! Данные %1 байт, буфер %2 байт")
//                 .arg(m_data.len).arg(m_data.bufsize),
//                 sv::log::llError, sv::log::mtError);

//    return raduga::TREATRESULT(DO_RESET);
//  }

//  memcpy(&m_data.data[0],     &p_io_buffer->input->data[m_hsz + 2], m_data.len);    // данные

//  quint16 calc_crc = CRC::MODBUS_CRC16((const quint8*)&p_io_buffer->input->data[0], m_hsz + m_header.byte_count); // вычисляем crc из данных

//  if(calc_crc != m_data.crc) {

//    // если crc не совпадает, то выходим без обработки и ответа
//    message(QString("Ошибка crc! Ожидалось %1%2, получено %3%4")
//                 .arg(quint8(calc_crc), 2, 16, QChar('0'))
//                 .arg(quint8(calc_crc >> 8), 2, 16, QChar('0'))
//                 .arg(quint8(m_data.crc), 2, 16, QChar('0'))
//                 .arg(quint8(m_data.crc >> 8), 2, 16, QChar('0')),
//                 sv::log::llError, sv::log::mtError);

//    return raduga::TREATRESULT(DO_RESET);
//  }

//  // если все корректно, то разбираем данные в зависимости от типа
//  switch (current_register - m_params.start_register)
//  {
//      case 0x00:
//      case 0x03:
//      case 0x05:

//        // здесь просто отправляем ответ-квитирование
//        confirmation();

//        if(m_data.type == 0x77) {

//          for (modus::SvSignal* signal: p_input_signals)
//            signal->setValue(0);
//        }

//        break;

//      case 0x06:
//      case 0x10:
//      case 0x50:
//      case 0x90:
//      {
//         // формируем и отправляем ответ-квитирование
//         confirmation();

//         if(signal_collections.contains(m_data.type))
//           signal_collections.value(m_data.type)->updateSignals(&m_data);

//         break;
//      }

//      default:
//          break;
//  }


  return raduga::TREATRESULT(DO_RESET, QDateTime::currentDateTime());
}

void raduga::SvRaduga::confirmation()
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

raduga::TREATRESULT raduga::SvRaduga::putout()
{
  switch (m_params.packid) {
  case 101:
  case 105:
  case 106:
  case 110:
    //data_type_1
    break;

  case 102:
  case 107:
    //data_type_2
    //data_type_3
    break;

  case 103:
  case 108:
    //data_type_5
    break;

  case 104:
  case 109:
    //data_type_9
    //data_type_53
    break;

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* device = new raduga::SvRaduga();
  return device;
}