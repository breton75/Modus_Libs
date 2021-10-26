#include "moxa.h"

apak::SvMoxa::SvMoxa():
  modus::SvAbstractProtocol(),
  m_data_signal(nullptr),
  m_state_signal(nullptr)
{

}

apak::SvMoxa::~SvMoxa()
{
  deleteLater();
}

bool apak::SvMoxa::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
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

bool apak::SvMoxa::bindSignal(modus::SvSignal* signal, modus::SignalBinding binding)
{
  try {

    bool r = modus::SvAbstractProtocol::bindSignal(signal, binding);

    if(r) {

      if(binding.mode == modus::Master) {

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
    }

    return r;
  }

  catch(SvException& e) {
    p_last_error = e.error;
    return false;
  }
}

void apak::SvMoxa::signalUpdated(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvMoxa::signalChanged(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvMoxa::start()
{
  if(!(bool(p_config) && bool(p_io_buffer))) {

    emit message(QString("Panic! Устройство %1 не проинициализировано").arg(p_config->name), sv::log::llError, sv::log::mtError);
    return false;
  }

  QTimer* m_timer = new QTimer;
  connect(m_timer, &QTimer::timeout, this, &apak::SvMoxa::sendRequest);
  m_timer->start(m_params.interval);

  connect(p_io_buffer, &modus::IOBuffer::dataReaded, this, &apak::SvMoxa::parse);

}

void apak::SvMoxa::sendRequest()
{
  p_io_buffer->output->mutex.lock();

//  0102 0000 0006 01 04 1000 000C

  QByteArray request = QByteArray::fromHex(QString("0101000000060104"));
  QDataStream s(&request, QIODevice::ReadWrite);
  s << m_params.start_register << m_params.register_count;

  quint16 crc = crc::crc16ccitt(ba);
  request.append(crc & 0xFF).append(crc >> 8);

  memcpy(p_io_buffer->output->data, request.data(), request.length());

  p_io_buffer->output->setReady(true);
  emit p_io_buffer->readyWrite(p_io_buffer->output);

  p_io_buffer->output->mutex.unlock();
}

void apak::SvMoxa::parse(modus::BUFF* buffer)
{
  buffer->mutex.lock();

  if(buffer->isReady()) {

    QByteArray ba(buffer->data, buffer->offset);
    QDataStream st(ba);
    MOXAReplay r;

    st >> r.id >> r.proto >> r.msglen >> r.address >> r.func >> r.bytecnt;

    QList<quint16> registers;

    for(quint16 reg = m_params.start_register; reg < r.bytecnt / 2; reg++) {

!!!

    }

    if(m_data_signal) {

      m_data_signal->setValue(QVariant(QByteArray(buffer->data, buffer->offset)));

      emit message(QString("signal %1 updated").arg(m_data_signal->config()->name), sv::log::llDebug, sv::log::mtParse);

    }

    if(m_state_signal)
      m_state_signal->setValue(int(1));

    buffer->reset();
  }
  else {

    checkupSignals();

  }

  buffer->mutex.unlock();

}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvMoxa();
  return protocol;
}

