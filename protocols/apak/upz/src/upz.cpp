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

    m_params = apak::ProtocolParams::fromJson(p_config->protocol.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool apak::SvUPZ::bindSignal(modus::SvSignal* signal, modus::SignalBinding binding)
{
  try {

    bool r = modus::SvAbstractProtocol::bindSignal(signal, binding);

    if(r) {

      if(binding.mode == modus::ReadWrite) {

        if(signal->config()->type.toLower() == "state") {

          if(m_state_signal)
            throw SvException(TOO_MUCH(p_config->name, "state"));

          else
            m_state_signal = signal;

        }
        else if(m_data_signal) {
          throw SvException(TOO_MUCH(p_config->name, "data"));

        }
        else
          m_data_signal = signal;

        return true;

      }
      else {

        connect(signal, &modus::SvSignal::updated, this, &SvUPZ::signalUpdated, Qt::QueuedConnection);
        connect(signal, &modus::SvSignal::changed, this, &SvUPZ::signalChanged, Qt::QueuedConnection);
      }
    }

    return r;

  }

  catch(SvException& e) {

    p_last_error = e.error;
    return false;
  }
}

void apak::SvUPZ::signalUpdated(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvUPZ::signalChanged(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvUPZ::start()
{
  p_is_active = bool(p_config) && bool(p_io_buffer);
  while(p_is_active) {

    qDebug() << 1;
    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

    if(p_io_buffer->input->ready()) {

      m_data_signal->setValue(QByteArray(p_io_buffer->input->data, p_io_buffer->input->offset));
      emit message(QString("signal %1 updated").arg(m_data_signal->config()->name), sv::log::llDebug, sv::log::mtParse);

      m_state_signal->setValue(int(1));

      p_io_buffer->input->reset();
    }

    p_io_buffer->input->mutex.unlock();
    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

//    p_io_buffer->output->mutex.lock();

//    putout();

//    p_io_buffer->output->mutex.unlock();

//    thread()->msleep(m_params.parse_interval);

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvUPZ();
  return protocol;
}
