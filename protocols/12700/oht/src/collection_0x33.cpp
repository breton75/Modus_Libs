#include "collection_0x33.h"

oht::Type0x33::Type0x33():
  SvAbstractSignalCollection()
{

}

void oht::Type0x33::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try
  {
    oht::SignalParams_0x33 p = oht::SignalParams_0x33::fromJson(signal->config()->params);

    m_signals.append(Signal0x33(signal, p));

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name, e.error));
  }
}

void oht::Type0x33::updateSignals(const oht::DATA* data)
{
  if(!data)
    return;

  foreach (Signal0x33 signal33, m_signals) {

    if(signal33.params.byte < data->data_length)
      signal33.signal->setValue(int((data->data[signal33.params.byte] >> signal33.params.bit) & 1));

  }
}
