#include "universal_packet.h"

apak::SvUniversalPack::SvUniversalPack():
  modus::SvAbstractProtocol(),
  m_in_signal(nullptr),
  m_job_signal(nullptr)
{

}

apak::SvUniversalPack::~SvUniversalPack()
{
  deleteLater();
}

bool apak::SvUniversalPack::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
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

void apak::SvUniversalPack::disposeSignal (modus::SvSignal* signal)
{
  uint uid = entid(P_DEVICE, p_config->id);
  if(!signal->config()->bindings.contains(uid))
    return;

  switch (signal->config()->bindings.value(uid).mode) {

    case modus::ReadWrite:
    {
      if(m_in_signal)
        throw SvException("К данному устройству может быть привязан только один сигнал с типом ReadWrite");

      m_in_signal = signal;

      break;
    }

    case modus::ReadOnly:
    {
      if(m_job_signal)
        throw SvException("К данному устройству может быть привязан только один сигнал с типом ReadOnly");

      m_job_signal = signal;

      break;
    }

    default:
      throw SvException(QString("Не могу привязать сигнал '%1' к устройству '%2'. Тип привязки не поддерживается.").arg(signal->config()->name).arg(p_config->name));
      break;
  }

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

void apak::SvUniversalPack::run()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);

  while(p_is_active) {

    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

    if(p_io_buffer->input->ready()) {

      m_in_signal->setValue(QByteArray(p_io_buffer->input->data, p_io_buffer->input->offset));
      emit message(QString("Signal '%1' updated").arg(m_in_signal->config()->name), sv::log::llDebug, sv::log::mtParse);

      p_io_buffer->input->reset();
    }

    p_io_buffer->input->mutex.unlock();
    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

//    p_io_buffer->output->mutex.lock();

//    putout();

//    p_io_buffer->output->mutex.unlock();

    msleep(m_params.parse_interval);

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvUniversalPack();
  return protocol;
}

