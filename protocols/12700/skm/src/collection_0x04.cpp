﻿#include "collection_0x04.h"

skm::Type0x04::Type0x04():
  SvAbstractSignalCollection()
{

}

void skm::Type0x04::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try
  {
    skm::SignalParams_0x04 p = skm::SignalParams_0x04::fromJson(signal->config()->params);

    m_signals.append(Signal0x04(signal, p));

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name).arg(e.error));
  }
}

void skm::Type0x04::updateSignals(const skm::DATA* data)
{
  if(!data)
    return;

  foreach (Signal0x04 signal04, m_signals) {

    if(signal04.params.byte < data->data_length)
      signal04.signal->setValue(int((data->data[signal04.params.byte] >> signal04.params.bit) & 1));

  }
}
