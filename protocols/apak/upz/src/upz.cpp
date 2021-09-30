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
  QTimer* m_timer = new QTimer;
  connect(m_timer, &QTimer::timeout, this, &SvUPZ::parse);
  m_timer->start(m_params.parse_interval);

  p_is_active = bool(p_config) && bool(p_io_buffer);
}

void apak::SvUPZ::parse()
{
  if(p_is_active) {

    p_io_buffer->confirm->mutex.lock();     // если нужен ответ квитирование
    p_io_buffer->input->mutex.lock();

    checkupSignals();

    if(p_io_buffer->input->ready()) {

      m_data_signal->setValue(QVariant(QByteArray(p_io_buffer->input->data, p_io_buffer->input->offset)));

      emit message(QString("signal %1 updated").arg(m_data_signal->config()->name), sv::log::llDebug, sv::log::mtParse);
      m_state_signal->setValue(int(1));

      p_io_buffer->input->reset();
    }
    else {

      /* алгоритм обновления значения сигнала:
         1. если для сиганала НЕ задан timeout, то есть timeout = 0, то смотрим время,
            когда последний раз происходил парсинг данных (last_parsed_time). если время парсинга данных не превышает
            timeout, то освежаем сигнал (обновляем last_update_time). другими словами - если timeout = 0, то освежаем сигнал
            пока last_parsed_time + DEFAULT_TIMEOUT меньше текущего времени

         2. если время обновления сигнала превышает timeout (сигнал утратил свежесть), то смотрим время,
            когда последний раз происходил парсинг данных (last_parsed_time). если время парсинга данных не превышает
            timeout, то освежаем сигнал (обновляем last_update_time). другими словами - если сигнал давно не обновлялся, а
            данные от устройства продолжают идти, считаем, что данный сигнал тоже актуален.

         3. если сигнал не утратил свежести, то есть врем обновления не превышает timeout, то и не трогаем его.
      */
      for (modus::SvSignal* signal: p_signals.keys()) {

        if(p_signals.value(signal).mode != modus::ReadWrite)
          continue;

        if(!signal->hasTimeout())
          signal->approve(p_last_parsed_time.toMSecsSinceEpoch() + DEFAULT_TIMEOUT < QDateTime::currentMSecsSinceEpoch());

        else if(signal->lastUpdate().toMSecsSinceEpoch() + signal->config()->timeout > QDateTime::currentMSecsSinceEpoch())
          signal->approve(p_last_parsed_time.toMSecsSinceEpoch() + signal->config()->timeout < QDateTime::currentMSecsSinceEpoch());

      }
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
  modus::SvAbstractProtocol* protocol = new apak::SvUPZ();
  return protocol;
}
