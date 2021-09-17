#include "hmi.h"

apak::SvHMI::SvHMI():
  modus::SvAbstractProtocol()
{

}

apak::SvHMI::~SvHMI()
{
  deleteLater();
}

bool apak::SvHMI::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = apak::ProtocolParams::fromJson(p_config->protocol.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void apak::SvHMI::disposeInputSignal (modus::SvSignal* signal)
{
  m_input_signals.insert(signal->config()->tag, signal);
}

void apak::SvHMI::disposeOutputSignal (modus::SvSignal* signal)
{
  m_output_signals.insert(signal->config()->tag, signal);
}

void apak::SvHMI::run()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);

  while(p_is_active) {

    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

    if(p_io_buffer->input->ready()) {
//    parse(p_io_buffer->input);
      p_io_buffer->input->reset();
    }

    p_io_buffer->input->mutex.unlock();
    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

    p_io_buffer->output->mutex.lock();
//    putout();
    p_io_buffer->output->mutex.unlock();

    msleep(m_params.parse_interval);

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvHMI();
  return protocol;
}
