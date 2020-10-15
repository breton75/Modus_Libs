﻿#include "collection_0x03.h"

opa::Type0x03::Type0x03():
  SvAbstractSignalCollection()
{

}

void opa::Type0x03::addSignal(SvSignal* signal) throw (SvException)
{
  try
  {
    opa::SignalParams_0x03 p = opa::SignalParams_0x03::fromJson(signal->config()->params);

    quint32 uniq_index = (static_cast<quint32>(p.space) << 8) + static_cast<quint32>(p.level);

    if(m_signals.contains(uniq_index))
      throw SvException(QString("Не уникальные значения параметров: '%1'")
                        .arg(signal->config()->params));

    m_signals.insert(uniq_index, signal);

  }
  catch(SvException e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name).arg(e.error));
  }
}

void opa::Type0x03::updateSignals(const ad::DATA* data)
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

      foreach (quint32 index, m_signals.keys()) {

        if((index >> 8) == space)
          m_signals.value(index)->setValue(0);

      }
    }

    data_begin += 4; // 4 байта данных на один извещатель

  }
}
