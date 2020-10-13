#include "collection_status.h"

opa::LineStatus::LineStatus():
  SvAbstractSignalCollection()
{

}

void opa::LineStatus::addSignal(SvSignal* signal) throw (SvException)
{
  try {

    m_signals.append(signal);

  } catch(SvException e) {
    throw e;
  }
}

void opa::LineStatus::updateSignals(const ad::DATA* data)
{
  Q_UNUSED(data);

  foreach (SvSignal* signal, m_signals)
    signal->setValue(1);

}
