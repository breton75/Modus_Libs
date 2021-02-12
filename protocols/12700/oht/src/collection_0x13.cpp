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

    quint32 uniq_index = (static_cast<quint32>(p.route) << 8) + static_cast<quint32>(p.number);

    if(m_signals.contains(uniq_index))
      throw SvException(QString("Не уникальные значения параметров: '%1'")
                        .arg(signal->config()->params));

    m_signals.insert(uniq_index, signal);

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

  quint8 data_begin = 0;
  quint8 route;

  while(data_begin < data->data_length) {

    memcpy(&route, &data->data[data_begin], 1);
//    memcpy(&level, &data->data[data_begin + 2], 1);

    QVariant value = QVariant();
    quint32 uniq_index = (static_cast<quint32>(route) << 8);

    for(int i = 1; i < 5; i++) {

      if(!m_signals.contains(uniq_index + i))
        continue;

      switch (i) {

        case 1:
          value = static_cast<quint8>(data->data[1] & 0x0F);
          break;

        case 2:
          value = static_cast<quint8>(data->data[1] >> 4);
          break;

        case 3:
          value = static_cast<quint8>((data->data[2] >> 4) & 0x03);
          break;

        case 4:
          value = static_cast<quint8>((data->data[2] >> 7) & 0x01);
          break;
      }

      m_signals.value(uniq_index + 1)->setValue(value);

    }

    data_begin += 4; // 4 байта данных на одно напрвление

  }
}
