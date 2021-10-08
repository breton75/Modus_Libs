#include "gamma_opa_imitator.h"

apak::SvGammaOpaImitator::SvGammaOpaImitator():
  modus::SvAbstractProtocol(),
  m_send_signal(nullptr)
{

}

apak::SvGammaOpaImitator::~SvGammaOpaImitator()
{
  deleteLater();
}

bool apak::SvGammaOpaImitator::configure(modus::DeviceConfig *config, modus::IOBuffer *iobuffer)
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

bool apak::SvGammaOpaImitator::bindSignal(modus::SvSignal* signal, modus::SignalBinding binding)
{
  try {

    bool r = modus::SvAbstractProtocol::bindSignal(signal, binding);

    if(r) {

      if(binding.mode == modus::ReadWrite) {


      }
      else {

        if(m_send_signal) {

          p_last_error = TOO_MUCH(p_config->name, "send");
          return false;
        }

        m_send_signal = signal;

        connect(signal, &modus::SvSignal::updated, this, &SvGammaOpaImitator::signalUpdated, Qt::QueuedConnection);
        connect(signal, &modus::SvSignal::changed, this, &SvGammaOpaImitator::signalChanged, Qt::QueuedConnection);
      }
    }

    return r;
  }

  catch(SvException& e) {
    p_last_error = e.error;
    return false;
  }
}

void apak::SvGammaOpaImitator::signalUpdated(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
//  // лочим мьютекс, чтобы синхронизироваться с отправкой
//  QList<QString> sl = QString(signal->value().toByteArray()).split("<CR><CL>");

//  while(!sem.tryAcquire())
//    qApp->processEvents();

//  send_data.clear();

//  for(QString s: sl) {

//    if(!(s.startsWith("$PPAS") && s.endsWith("<CRC>")))
//      continue;

//    // контрольная сумма
//    // восьмибитная XOR-сумма всех символов (включая «,» и «^») в строке между «$» и «*»,
//    // приведенная к двум ASCII-символам в верхнем регистре для 16-ричного представления байта (0–9, A–F).
//    uchar crc = 0x00;
//    QString d = s.mid(1, s.length() - s.indexOf("*"));

//    for(QChar c: d)
//      crc ^= c.cell();

//    s.replace("<CRC>", QString(QByteArray(1, crc).toHex()));

//    send_data.append(s).append(QChar(0x0D)).append(QChar(0x0A));

//  }

//  sem.release();

}

void apak::SvGammaOpaImitator::signalChanged(modus::SvSignal* signal)
{
  Q_UNUSED(signal);
}

void apak::SvGammaOpaImitator::start()
{
  m_timer = new QTimer;
  connect(m_timer, &QTimer::timeout, this, &SvGammaOpaImitator::send);
  m_timer->start(m_params.send_interval);

  p_is_active = bool(p_config) && bool(p_io_buffer) && bool(m_send_signal);
}

void apak::SvGammaOpaImitator::send()
{
  if(p_is_active) {

    QList<QString> sl = QString(m_send_signal->value().toByteArray()).split("<CR><LF>");

    send_data.clear();

    for(QString s: sl) {

      if(!(s.startsWith("$PPAS") && s.endsWith("<CRC>")))
        continue;

      // контрольная сумма
      // восьмибитная XOR-сумма всех символов (включая «,» и «^») в строке между «$» и «*»,
      // приведенная к двум ASCII-символам в верхнем регистре для 16-ричного представления байта (0–9, A–F).
      uchar crc = 0x00;
      QString d = s.mid(1, s.length() - s.indexOf("*"));

      for(QChar c: d)
        crc ^= c.cell();

      s.replace("<CRC>", QString(QByteArray(1, crc).toHex()));

      send_data.append(s).append(QChar(0x0D)).append(QChar(0x0A));

    }

    p_io_buffer->output->mutex.lock();

    memcpy(&p_io_buffer->output->data[0], send_data.data(), send_data.length());
    p_io_buffer->output->offset = send_data.length();

    emit message(QString(send_data), lldbg, sv::log::mtNew);

    p_io_buffer->output->mutex.unlock();

    emit p_io_buffer->readyWrite(p_io_buffer->output);

  }
}

/** ********** EXPORT ************ **/
modus::SvAbstractProtocol* create()
{
  modus::SvAbstractProtocol* protocol = new apak::SvGammaOpaImitator();
  return protocol;
}

