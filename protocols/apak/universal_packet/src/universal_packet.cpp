#include "universal_packet.h"

apak::SvUniversalPack::SvUniversalPack():
  modus::SvAbstractProtocol(),
  m_data_signal(nullptr),
  m_state_signal(nullptr)
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

bool apak::SvUniversalPack::bindSignal(modus::SvSignal* signal, modus::SignalBinding binding)
{
  try {

    bool r = modus::SvAbstractProtocol::bindSignal(signal, binding);

    if(r) {

      if(binding.mode == modus::ReadWrite) {

        if(signal->config()->type.toLower() == "data") {

          if(m_data_signal) {

            p_last_error = TOO_MUCH(p_config->name, "data");
            return false;
          }

          m_data_signal = signal;

        }
        else if(signal->config()->type.toLower() == "state") {

          if(m_state_signal) {

            p_last_error = TOO_MUCH(p_config->name, "state");
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

    return r;
  }

  catch(SvException& e) {
    p_last_error = e.error;
    return false;
  }
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
  QTimer* m_timer = new QTimer;
  connect(m_timer, &QTimer::timeout, this, &SvUniversalPack::parse);
  m_timer->start(m_params.parse_interval);

  p_is_active = bool(p_config) && bool(p_io_buffer);
}

void apak::SvUniversalPack::parse()
{
  if(p_is_active) {

    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

    if(p_io_buffer->input->ready()) {

      m_data_signal->setValue(QVariant(QByteArray(p_io_buffer->input->data, p_io_buffer->input->offset)));

      emit message(QString("signal %1 updated").arg(m_data_signal->config()->name), sv::log::llDebug, sv::log::mtParse);
      m_state_signal->setValue(int(1));

      p_io_buffer->input->reset();
    }
    else {

      checkupSignals();

    }

    p_io_buffer->input->mutex.unlock();
    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

//    p_io_buffer->output->mutex.lock();
//    putout();
//    p_io_buffer->output->mutex.unlock();

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvUniversalPack();
  return protocol;
}

