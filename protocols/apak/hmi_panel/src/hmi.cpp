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

    m_params = hmi::ProtocolParams::fromJson(p_config->protocol.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool apak::SvHMI::bindSignal(modus::SvSignal *signal, modus::SignalBinding binding)
{
  try {

    bool r = modus::SvAbstractProtocol::bindSignal(signal, binding);

    if(r) {

      hmi::SignalParams params = hmi::SignalParams::fromJson(binding.params);

      if(binding.mode == modus::ReadWrite) {

        m_input_signals.insert(signal, params);

      }
      else
        m_output_signals.insert(signal, params);

    }

    return r;

  }
  catch(SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void apak::SvHMI::signalUpdated(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvHMI::signalChanged(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvHMI::start()
{
  QTimer* m_timer = new QTimer;
  connect(m_timer, &QTimer::timeout, this, &SvHMI::putout);
  m_timer->start(m_params.interval);

//  connect(p_io_buffer, &modus::IOBuffer::dataReaded, this, &SvGammaOpaImitator::parse);

  p_is_active = bool(p_config) && bool(p_io_buffer);
}

void apak::SvHMI::putout()
{
  QMap<quint16, quint16> dataByRegisters;
  for(modus::SvSignal* signal: m_output_signals.keys()) {

    hmi::SignalParams params = m_output_signals.value(signal);

    bool ok;
    quint16 v = signal->value().toUInt(&ok);
    if(!ok)
      continue;

    quint16 mask = ~(((1 << params.len) - 1) << params.offset); // даже если больше 1го бита
    v = mask | (v << params.offset);

    if(dataByRegisters.contains(params.registr))
      dataByRegisters.insert(params.registr, 0);

    dataByRegisters[params.registr] |= v;

  }
qDebug() << 100;
  foreach (quint16 registr, dataByRegisters.keys()) {

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << quint8(m_params.address)                  // Адрес устройства
           << quint8(0x0F)                              // Функциональный код     у нас всегда 0x0F
           << registr                                   // Адрес первого регистра
           << quint16(1)                                // Количество регистров
           << quint16(2)                                // Количество байт далее
           << quint16(dataByRegisters.value(registr));  // Значение байт

    quint16 crc = CRC::MODBUS_CRC16((const quint8*)data.data(), data.length());

    stream << crc;                                      // Контрольная сумма CRC

    p_io_buffer->output->mutex.lock();

    memcpy(&p_io_buffer->output[0], data.data(), data.length());
    p_io_buffer->output->offset = data.length();

    p_io_buffer->output->mutex.unlock();

    emit p_io_buffer->readyWrite(p_io_buffer->output);

  }


}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvHMI();
  return protocol;
}
