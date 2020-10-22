#include "collection_0x02.h"

opa::Type0x02::Type0x02():
  SvAbstractSignalCollection()
{

}

void opa::Type0x02::addSignal(SvSignal* signal) throw (SvException)
{
  try
  {
    opa::SignalParams_0x02 p = opa::SignalParams_0x02::fromJson(signal->config()->params);

    quint32 uniq_index = (static_cast<quint32>(p.sensor) << 8) + static_cast<quint32>(p.faktor);

//    if(m_signals.contains(uniq_index))
//      throw SvException(QString("Не уникальные значения параметров: '%1'")
//                        .arg(signal->config()->params));

    m_signals.insert(uniq_index, signal);

  }
  catch(SvException e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name).arg(e.error));
  }
}

void opa::Type0x02::updateSignals(const opa::DATA* data)
{
  if(!data)
    return;

  quint8  data_begin = 0;
  quint16 sensor;
  quint8  faktor;

  while(data_begin < data->data_length) {

    memcpy(&sensor, &data->data[data_begin], 2);
    memcpy(&faktor, &data->data[data_begin + 2], 1);

    if(faktor) {

      quint32 uniq_index = (static_cast<quint32>(sensor) << 8) + static_cast<quint32>(faktor);

      if(m_signals.contains(uniq_index)) {
        for (SvSignal* signal: m_signals.values(uniq_index))
          signal->setValue(1);
      }

    }

    else {

      foreach (quint32 index, m_signals.keys()) {

        if((index >> 8) == sensor){
          for (SvSignal* signal: m_signals.values(index))
            signal->setValue(0);
        }
      }
    }

    data_begin += 4; // 4 байта данных на один извещатель

  }
}
