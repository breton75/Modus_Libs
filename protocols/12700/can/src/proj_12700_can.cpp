#include "proj_12700_can.h"

can::SvCAN12700::SvCAN12700():
  modus::SvAbstractProtocol()
{
//  signal_collections.insert(TYPE_0x01, &type0x01_signals);
//  signal_collections.insert(TYPE_0x02, &type0x02_signals);
}

bool can::SvCAN12700::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

//    m_params = can::DeviceParams::fromJson(p_config->protocol.params);

    if(!m_data.resize(p_config->bufsize))
      throw SvException(QString("Не удалось выделить %1 байт памяти для буфера").arg(p_config->bufsize));

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void can::SvCAN12700::disposeInputSignal (modus::SvSignal* signal)
{
  try {

    signal_collection.addSignal(signal);

  }

  catch(SvException& e) {
    throw e;
  }
}

void can::SvCAN12700::disposeOutputSignal (modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void can::SvCAN12700::run()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);

  while(p_is_active) {

//    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

//    can::PARSERESULT result = parse();

//    if(result.do_reset == DO_RESET)
//      p_io_buffer->input->reset();

    p_io_buffer->input->mutex.unlock();
//    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

//    if(result.parse_time.isValid())
//      validateSignals(result.parse_time);

  }
}

can::PARSERESULT can::SvCAN12700::parse()
{

//  // проверяем, что длина данных в буфере не меньше длины заголовка
//  if(p_io_buffer->input->offset < m_hsz)
//    return can::PARSERESULT(DO_NOT_RESET);

//  canfd_frame frame;
//  frame.

//  // ищем признак конца пакета

//  while(!found && offset_of_2f55 < p_io_buffer->input->offset) {

//      found = (p_io_buffer->input->data[offset_of_2f55] == 0x2F) && (p_io_buffer->input->data[offset_of_2f55 + 1] == 0x55);
//      offset_of_2f55++;
//  }

//  if(!found)
//    return can::PARSERESULT(DO_NOT_RESET);

//  // если нашли конец пакета, то начинаем парсить его
//  {
//    p_io_buffer->input->offset = offset_of_2f55 + 1;

//    message(QString(QByteArray((const char*)&p_io_buffer->input->data[0], p_io_buffer->input->offset).toHex()));

//    // вытаскиваем данные
//    // длина данных вычисляется по вормуле:
//    // len = [длина пакета] - [длина заголовка] - [1 байт тип данных] - [crc 2 байта] - [2F55 в конце]
//    memcpy(&m_data.len,     &p_io_buffer->input->offset - (m_hsz + 1 + 2 + 2), 1);
//    memcpy(&m_data.type,    &p_io_buffer->input->data[m_hsz], 1);
//    memcpy(&m_data.crc,     &p_io_buffer->input->data[p_io_buffer->input->offset - 4], 2);

//    // проверяем, что длина данных не превышает размер выделенного буфера
//    if(m_data.len > m_data.bufsize) {

//      message(QString("Размер данных превышает размер буфера! Данные %1 байт, буфер %2 байт")
//                   .arg(m_data.len).arg(m_data.bufsize),
//                   sv::log::llError, sv::log::mtError);

//      return can::PARSERESULT(DO_RESET);
//    }

//    memcpy(&m_data.data[0], &p_io_buffer->input->data[m_hsz], m_data.len);          // данные

//    /* программист СКМ говорит, что они никак не анализируют мой ответ на посылку данных
//     * поэтому, чтобы не тратить ресурсы, убрал отправку подтверждения.
//     * к тому же неправильно считается crc. надо разбираться и переделывать
//    if(!sendConfirmation()) return;
//    */

//    if(signal_collections.contains(m_data.type))
//      signal_collections.value(m_data.type)->updateSignals(&m_data);

//  }

  return can::PARSERESULT(DO_RESET, QDateTime::currentDateTime());
}


/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* device = new can::SvCAN12700();
  return device;
}
