#include "type_0x02.h"

Type0x02::Type0x02(QObject *parent) : QObject(parent)
{

}

void Type0x02::addSignal(const SvSignal* signal) throw (SvException)
{
  try
  {
    SignalParams_0x02 p = SignalParams_0x02::fromJson(signal->config()->params);

    quint32 uniq_index = (static_cast<quint32>(p.sensor) << 8) + static_cast<quint32>(p.faktor);

    if(m_signals.contains(uniq_index))
      throw SvException(QString("Сигнал %1: не уникальные значения параметров: '%2'")
                        .arg(signal->config()->name)
                        .arg(signal->config()->params));

    m_signals.insert(uniq_index, signal->config()->name);

  }
  catch(SvException e)
  {
    throw e;
  }
}

void Type0x02::updateSignals(const ad::DATA* data)
{
  quint8  data_begin = 0;
  quint16 sensor;
  quint8  factor;
//  QString signal_name;

  while(data_begin < data->data_length) {

    memcpy(&sensor, &data->data[data_begin], 2);
    memcpy(&factor, &data->data[data_begin + 2], 1);

    if(factor) {

      quint32 uniq_index = (static_cast<quint32>(sensor) << 8) + static_cast<quint32>(faktor);

      if(m_signals.contains(uniq_index))
        device->setSignalValue(m_signals.value(uniq_index), 1);

    }

    else {

      foreach (quint32 index, m_signals.keys()) {

        if((index >> 8) == sensor)
          device->setSignalValue(m_signals.value(index), 0);

      }
    }

    data_begin += 4; // 4 байта данных на один извещатель

  }
}
