#include "collection_0x13.h"

oht::Type0x13::Type0x13():
  SvAbstractSignalCollection()
{

}

void oht::Type0x13::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try
  {
    oht::SignalParams_0x13 p = oht::SignalParams_0x13::fromJson(signal->config()->params);

    quint32 uid = getUid(0, p.route, p.byte, p.bit);

    if(m_signals.contains(uid))
      throw SvException(QString("Не уникальные значения параметров: '%1'")
                        .arg(signal->config()->params));

    m_signals.insert(uid, oht::Signal0x13(signal, p));

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name, e.error));
  }
}

void oht::Type0x13::updateSignals(const oht::DATA* data)
{
  if(!data)
    return;

  quint8 offset = 0;
  quint8 route;

  while(data->len - offset >= ROUTE_DATA_LENGTH) {

    route = data->data[offset];

    for(int byte = offset + 1; byte < offset + ROUTE_DATA_LENGTH; byte++) {

      for(int bit = 0; bit < 8; bit++) {

        quint32 uid   = getUid(0, route, byte, bit);

        if(!m_signals.contains(uid))
          continue;

        quint8 mask = pow(2, m_signals.value(uid).params.len) - 1;
        QVariant value = (static_cast<quint8>(data->data[byte] >> bit) & mask);

        m_signals.value(uid).signal->setValue(value);

      }
    }

    offset += ROUTE_DATA_LENGTH; // 4 байта данных на одно напрвление

  }
}
