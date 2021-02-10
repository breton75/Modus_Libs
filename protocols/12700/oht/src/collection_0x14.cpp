#include "collection_0x14.h"

oht::Type0x14::Type0x14():
  SvAbstractSignalCollection()
{

}

void oht::Type0x14::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try
  {
    oht::SignalParams_0x14 p = oht::SignalParams_0x14::fromJson(signal->config()->params);

    m_signals.append(Signal0x14(signal, p));

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name, e.error));
  }
}

void oht::Type0x14::updateSignals(const oht::DATA* data)
{
  if(!data)
    return;

  foreach (Signal0x14 signal14, m_signals) {

    if(signal14.params.byte < data->data_length)
      signal14.signal->setValue(int((data->data[signal14.params.byte] >> signal14.params.bit) & 1));

  }
}
