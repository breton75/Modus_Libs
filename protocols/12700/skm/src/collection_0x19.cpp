#include "collection_0x19.h"

skm::Type0x19::Type0x19():
  SvAbstractSignalCollection()
{

}

void skm::Type0x19::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try
  {
    skm::SignalParams_0x19 p = skm::SignalParams_0x19::fromJson(signal->config()->params);

    m_signals.append(Signal0x19(signal, p));

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name).arg(e.error));
  }
}

void skm::Type0x19::updateSignals(const skm::DATA* data)
{
  if(!data)
    return;

  foreach (Signal0x19 signal19, m_signals) {

    if(signal19.params.byte < data->data_length)
      signal19.signal->setValue(int((data->data[signal19.params.byte] ))); // проверка битов делается в агрегате  >> signal19.params.bit) & 1));

  }
}
