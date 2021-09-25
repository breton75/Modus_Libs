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

bool apak::SvUniversalPack::bindSignal(modus::SvSignal* signal, modus::BindMode mode)
{
  if(!p_signals.contains(signal)) {

    p_signals.append(signal);

    if(mode == modus::ReadWrite) {

      if(signal->config()->type.toLower() == "data") {

        if(m_data_signal) {

          p_last_error = "К данному устройству может быть привязан только один сигнал с типом 'data'";
          return false;
        }

        m_data_signal = signal;

      }
      else if(signal->config()->type.toLower() == "state") {

        if(m_state_signal) {

          p_last_error = "К данному устройству может быть привязан только один сигнал с типом 'state'";
          return false;
        }

        m_state_signal = signal;

      }
    }
    else {

      connect(signal, &modus::SvSignal::updated, this, &SvUniversalPack::signalUpdated, Qt::QueuedConnection);
      connect(signal, &modus::SvSignal::changed, this, &SvUniversalPack::signalChanged, Qt::QueuedConnection);
    }
  }

  return  true;
}

void apak::SvUniversalPack::signalUpdated(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvUniversalPack::signalChanged(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvUniversalPack::start()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);

  while(p_is_active) {

    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

    if(p_io_buffer->input->ready()) {

      m_data_signal->setValue(QByteArray(p_io_buffer->input->data, p_io_buffer->input->offset));
      emit message(QString("Signal '%1' updated").arg(m_data_signal->config()->name), sv::log::llDebug, sv::log::mtParse);

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

