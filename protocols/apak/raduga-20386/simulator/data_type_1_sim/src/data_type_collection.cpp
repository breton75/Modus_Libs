#include "data_type_collection.h"

raduga::DataTypeCollection::DataTypeCollection():
  SvAbstractSignalCollection()
{

}

void raduga::DataTypeCollection::addSignal(modus::SvSignal* signal)
{
  try
  {
    raduga::SignalParams p = raduga::SignalParams::fromJson(signal->config()->params);

    m_signals.append(raduga::SignalStruct(signal, p));

  }
  catch(SvException& e)
  {
    throw SvException(QString("Сигнал %1: %2").arg(signal->config()->name, e.error));
  }
}

void raduga::DataTypeCollection::updateSignals(const raduga::DATA* data)
{
  if(!data)
    return;

  foreach (raduga::SignalStruct st, m_signals) {

    if(st.params.byte < data->len)
      st.signal->setValue(int((data->data[st.params.byte] >> st.params.bit) & 1));

  }
}
