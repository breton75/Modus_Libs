#include "upz.h"

apak::SvUPZ::SvUPZ():
  modus::SvAbstractProtocol(),
  m_data_signal(nullptr),
  m_state_signal(nullptr)
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
        else {

          m_data_signal = signal;
        }

        return true;

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
//  QTimer* m_timer = new QTimer;
//  connect(m_timer, &QTimer::timeout, this, &SvUPZ::parse);
//  m_timer->start(m_params.parse_interval);

  connect(p_io_buffer, &modus::IOBuffer::dataReaded, this, &SvUPZ::parse);

  p_is_active = bool(p_config) && bool(p_io_buffer);
}

void apak::SvUPZ::parse(modus::BUFF* buffer)
{
  if(p_is_active) {

    buffer->mutex.lock();     // если нужен ответ квитирование
//    p_io_buffer->input->mutex.lock();

    if(buffer->ready()) {

      m_data_signal->setValue(QVariant(QByteArray(buffer->data, buffer->offset)));

      emit message(QString("signal %1 updated").arg(m_data_signal->config()->name), sv::log::llDebug, sv::log::mtParse);
      m_state_signal->setValue(int(1));

      buffer->reset();
    }
    else {

      checkupSignals();

    }

    buffer->mutex.unlock();
//    p_io_buffer->confirm->mutex.unlock();   // если нужен ответ квитирование

//    p_io_buffer->output->mutex.lock();
//    putout();
//    p_io_buffer->output->mutex.unlock();

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvUPZ();
  return protocol;
}
