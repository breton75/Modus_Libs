#include "can12700_signal.h"

can::CANSignalCollection::CANSignalCollection():
  SvAbstractSignalCollection()
{

}

void can::CANSignalCollection::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try
  {
    can::SignalParams p = can::SignalParams::fromJson(signal->config()->params);

    if(m_signals.contains(getUid(p.canid, 0, 0, quint8(p.offset / 8), p.offset % 8)))
      throw SvException(QString("Не уникальные значения параметров: '%1'")
                        .arg(signal->config()->params));

    m_signals.insert(getUid(p.canid, 0, 0, int(p.offset / 8), p.offset % 8), CANSignal(signal, p));

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name, e.error));
  }
}

void can::CANSignalCollection::updateSignals(const modus::DATA* data)
{
  Q_UNUSED(data);
}

void can::CANSignalCollection::updateSignals(const can_frame& frame)
{
  quint64 uid;

  for(int byte = 0; byte < frame.can_dlc; byte++) {
    for(int bit = 0; bit < 8; bit++) {

      uid = getUid(frame.can_id, 0, 0, byte, bit);

      if(!m_signals.contains(uid))
        continue;

      quint8 mask = pow(2, m_signals.value(uid).params.len) - 1;

      m_signals.value(uid).signal->setValue((frame.data[byte] >> bit) & mask);

    }
  }
}
