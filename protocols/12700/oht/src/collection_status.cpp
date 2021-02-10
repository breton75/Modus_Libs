#include "collection_status.h"

oht::LineStatus::LineStatus():
  SvAbstractSignalCollection()
{

}

void oht::LineStatus::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try {

    m_signals.append(signal);

  } catch(SvException& e) {
    throw e;
  }
}

void oht::LineStatus::updateSignals(const oht::DATA* data)
{
  Q_UNUSED(data);

  foreach (modus::SvSignal* signal, m_signals)
    signal->setValue(1);

}
