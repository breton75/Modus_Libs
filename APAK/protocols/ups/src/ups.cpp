#include "ups.h"

apak::SvUPS::SvUPS():
  modus::SvAbstractProtocol()
{

}

apak::SvUPS::~SvUPS()
{
  deleteLater();
}

bool apak::SvUPS::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = ups::ProtocolParams::fromJson(p_config->protocol.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

bool apak::SvUPS::bindSignal(modus::SvSignal *signal, modus::SignalBinding binding)
{
  try {

    bool r = modus::SvAbstractProtocol::bindSignal(signal, binding);

    if(r) {

      if(binding.mode == modus::Master) {

        //! параметры params для каждого свои! для master'a свои, а для bindings - свои
        ups::SignalParams params = ups::SignalParams::fromJson(binding.params);

        m_params_by_signals.insert(signal, params);

        if(!m_signals_by_registers.contains(params.registr))
          m_signals_by_registers.insert(params.registr, QList<modus::SvSignal*>());

        m_signals_by_registers[params.registr].append(signal);

        m_input_signals.insert(signal, ups::SignalParams());

      }
      else {


      }
    }

    return r;

  }
  catch(SvException& e) {

    p_last_error = e.error;
    return false;

  }
}

void apak::SvUPS::signalUpdated(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvUPS::signalChanged(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvUPS::start()
{
  QTimer* m_timer = new QTimer;
  connect(m_timer, &QTimer::timeout, this, &SvUPS::queue);
  m_timer->start(m_params.interval);

  connect(p_io_buffer, &modus::IOBuffer::dataReaded, this, &apak::SvUPS::parse);
  connect(this, &SvUPS::nextRequest, this, &apak::SvUPS::request, Qt::QueuedConnection); //! Queued

  p_is_active = bool(p_config) && bool(p_io_buffer);
}

void apak::SvUPS::queue()
{
  if(!send_queue.isEmpty())
    return;

  for(quint16 registr: m_signals_by_registers.keys())
    send_queue.append(registr);

  request();

}

void apak::SvUPS::request()
{
  if(send_queue.isEmpty())
    return;

  quint16 registr = send_queue.first();

  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);

  stream << quint8(m_params.address)               // Адрес устройства
         << quint8(m_params.func_code)             // Функциональный код. у нас 0x04
         << registr                                // Адрес первого регистра
         << quint16(1);                            // Количество регистров


  quint16 crc = crc::crc16ibm((unsigned char*)(data.data()), data.length()); // Контрольная сумма CRC
  data.append(quint8(crc & 0xFF));
  data.append(quint8(crc >> 8));

  p_io_buffer->output->mutex.lock();

  p_io_buffer->output->setData(data);

  emit p_io_buffer->readyWrite(p_io_buffer->output);

  p_io_buffer->output->mutex.unlock();

}

void apak::SvUPS::parse(modus::BUFF* buffer)
{
  if(!buffer->isReady())
    return;

  QByteArray data(buffer->data, buffer->offset);
  QDataStream stream(data);
  apak::UPSData upsdata;

  stream >> upsdata.address >> upsdata.func >> upsdata.bytecnt >> upsdata.value >> upsdata.crc;

  quint16 registr = send_queue.dequeue();

  if(m_signals_by_registers.contains(registr))
    m_signals_by_registers[registr].

  emit nextRequest();
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvUPS();
  return protocol;
}
