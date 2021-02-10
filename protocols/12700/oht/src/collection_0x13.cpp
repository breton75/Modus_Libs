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

    quint32 uniq_index = (static_cast<quint32>(p.room) << 8) + static_cast<quint32>(p.level);

    if(m_signals.contains(uniq_index))
      throw SvException(QString("Не уникальные значения параметров: '%1'")
                        .arg(signal->config()->params));

    m_signals.insert(uniq_index, signal);

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name).arg(e.error));
  }
}

void oht::Type0x13::updateSignals(const oht::DATA* data)
{
  if(!data)
    return;

  quint8  data_begin = 0;
  quint16 space;
  quint8  level;

  while(data_begin < data->data_length) {

    memcpy(&space, &data->data[data_begin], 2);
    memcpy(&level, &data->data[data_begin + 2], 1);

    if(level) {

      quint32 uniq_index = (static_cast<quint32>(space) << 8) + static_cast<quint32>(level);

      if(m_signals.contains(uniq_index))
        m_signals.value(uniq_index)->setValue(1);

    }

    else {

      foreach (quint32 uniq_index, m_signals.keys()) {

        if((uniq_index >> 8) == space)
          m_signals.value(uniq_index)->setValue(0);

      }
    }

    data_begin += 4; // 4 байта данных на один извещатель

  }
}
