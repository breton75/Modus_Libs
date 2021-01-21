#include "collection_status.h"

opa::LineStatus::LineStatus():
  SvAbstractSignalCollection()
{

}

void opa::LineStatus::addSignal(modus::SvSignal* signal) throw (SvException)
{
  try {

    m_signals.append(signal);

  } catch(SvException e) {
    throw e;
  }
}

void opa::LineStatus::updateSignals(const opa::DATA* data)
{
  Q_UNUSED(data);

  foreach (modus::SvSignal* signal, m_signals)
    signal->setValue(1);

}
