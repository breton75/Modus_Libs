#include "dummy.h"

plug::SvDummy::SvDummy():
  modus::SvAbstractProtocol()
{

}

plug::SvDummy::~SvDummy()
{
  deleteLater();
}

bool plug::SvDummy::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = plug::ProtocolParams::fromJson(p_config->protocol.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool plug::SvDummy::bindSignal(modus::SvSignal* signal, modus::SignalBinding binding)
{
  try {

    bool r = modus::SvAbstractProtocol::bindSignal(signal, binding);

    plug::SignalParams signal_params = plug::SignalParams::fromJson(binding.params);

    if(r) {

      if(binding.mode == modus::Master) {

        if(!m_params_by_signal.contains(signal))
          m_params_by_signal.insert(signal, signal_params);

      }
      else {

//        if(!m_signals_by_vin.contains(signal_params.vin))
//          m_signals_by_vin.insert(signal_params.vin, QList<modus::SvSignal*>());

//        m_signals_by_vin[signal_params.vin].append(signal);

      }
    }

    return r;
  }

  catch(SvException& e) {
    p_last_error = e.error;
    return false;
  }
}

void plug::SvDummy::signalUpdated(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void plug::SvDummy::signalChanged(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void plug::SvDummy::start()
{
//  m_timer = new QTimer;
//  connect(m_timer, &QTimer::timeout, this, &SvDummy::send);
//  m_timer->start(m_params.interval);

  connect(p_io_buffer, &modus::IOBuffer::dataReaded, this, &SvDummy::parse);

  p_is_active = bool(p_config) && bool(p_io_buffer);
}

void plug::SvDummy::parse(modus::BUFF* buffer)
{
  if(p_is_active) {

    buffer->mutex.lock();

    if(buffer->isReady()) {

      emit message(QString("data parsed: %1").arg(QString(QByteArray((const char*)&buffer->data[0], buffer->offset).toHex())), sv::log::llDebug, sv::log::mtParse);

      buffer->reset();

    }

    buffer->mutex.unlock();

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new plug::SvDummy();
  return protocol;
}

