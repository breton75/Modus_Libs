#include "upz.h"

apak::SvUPZ::SvUPZ():
  modus::SvAbstractProtocol()
{

}

apak::SvUPZ::~SvUPZ()
{
  deleteLater();
}

bool apak::SvUPZ::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

//    m_params = apak::ProtocolParams::fromJson(p_config->protocol.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void apak::SvUPZ::disposeInputSignal (modus::SvSignal* signal)
{
  m_signal = signal;

//  try {

//    bool ok;
//    quint16 type = signal->config()->type.toUInt(&ok, 0);

//    if(ok && input_signal_collections.contains(type))
//      input_signal_collections.value(type)->addSignal(signal, p_config->bufsize);

//    else {

//      emit message(QString("Сигнал %1: Неопознанный тип данных \"%2\"").arg(signal->config()->name).arg(signal->config()->type),
//                   sv::log::llError, sv::log::mtError);

//    }
//  }

//  catch(SvException& e) {
//    throw e;
//  }
}

void apak::SvUPZ::disposeOutputSignal (modus::SvSignal* signal)
{
  Q_UNUSED(signal);

//  try {

//    bool ok;
//    quint16 type = signal->config()->type.toUInt(&ok, 0);

//    if(ok && output_signal_collections.contains(type))
//      output_signal_collections.value(type)->addSignal(signal, p_config->bufsize);

//    else {

//      emit message(QString("Сигнал %1: Неопознанный тип данных \"%2\"").arg(signal->config()->name).arg(signal->config()->type),
//                   sv::log::llError, sv::log::mtError);

//    }
//  }

//  catch(SvException& e) {
//    throw e;
//  }
}

void apak::SvUPZ::run()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);

  qint64 estimate;
  while(p_is_active) {

    estimate = QDateTime::currentMSecsSinceEpoch() + m_params.parse_interval;

    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

    if(p_io_buffer->input->ready()) {

      m_signal->setValue(QByteArray(p_io_buffer->input->data, p_io_buffer->input->offset));
      p_io_buffer->input->reset();
    }

    p_io_buffer->input->mutex.unlock();
    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

//    p_io_buffer->output->mutex.lock();

//    putout();

//    p_io_buffer->output->mutex.unlock();

//    emit message("protocol before yield", sv::log::llDebug, sv::log::mtReceive);
//    QThread::yieldCurrentThread();
    if(p_is_active) {
      while (QDateTime::currentMSecsSinceEpoch() < estimate)
        qApp->processEvents();
    }

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvUPZ();
  return protocol;
}
