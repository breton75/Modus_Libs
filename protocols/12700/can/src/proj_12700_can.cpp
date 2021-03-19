#include "proj_12700_can.h"

can::SvCAN12700::SvCAN12700():
  modus::SvAbstractProtocol()
{

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

    can::PARSERESULT result = parse();

    if(result.do_reset == DO_RESET)
      p_io_buffer->input->reset();

    p_io_buffer->input->mutex.unlock();
//    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

    if(result.parse_time.isValid())
      validateSignals(result.parse_time);

    msleep(1);

  }
}

can::PARSERESULT can::SvCAN12700::parse()
{
//  qDebug() << p_io_buffer->input->offset << m_framesz;

  // проходим по буферу, с учетом того, что там могут быть несколько фреймов
  for(quint64 oof = 0; oof < p_io_buffer->input->offset; oof += m_framesz) {

    can_frame frame;
    memcpy(&frame, &p_io_buffer->input->data[oof], m_framesz);

    message(QString()
                .append(QDateTime::currentDateTime().toString("hhmmss.zzz"))
                .append(";")
                .append(QByteArray((const char*)&p_io_buffer->input->data[oof], 1).toHex())
            .append((";"))
            .append(QByteArray((const char*)&p_io_buffer->input->data[oof+1], 1).toHex())
            .append((";"))
            .append(QByteArray((const char*)&p_io_buffer->input->data[oof + 2], m_framesz - 2).toHex()));

    signal_collection.updateSignals(frame);

  }

  return can::PARSERESULT(DO_RESET, QDateTime::currentDateTime());
}


/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  return new can::SvCAN12700();
}
