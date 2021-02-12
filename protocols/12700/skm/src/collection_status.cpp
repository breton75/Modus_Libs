#include "collection_status.h"

skm::LineStatus::LineStatus():
  SvAbstractSignalCollection()
{

}

void skm::LineStatus::addSignal(modus::SvSignal* signal) //throw (SvException)
{
  try {

    m_signals.append(signal);

  } catch(SvException& e) {
    throw e;
  }
}

void skm::LineStatus::updateSignals(const skm::DATA* data)
{
  Q_UNUSED(data);

  foreach (modus::SvSignal* signal, m_signals)
    signal->setValue(1);

}
