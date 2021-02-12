#ifndef COLLECTION_OPA_LINE_STATUS_H
#define COLLECTION_OPA_LINE_STATUS_H

#include <QObject>
#include <QList>

#include "skm_defs.h"

namespace skm {

  class LineStatus : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit LineStatus();

    void addSignal(modus::SvSignal* signal); //throw (SvException);

    void updateSignals(const skm::DATA* data = nullptr);

  private:
    QList<modus::SvSignal*> m_signals;

  };
}


#endif // COLLECTION_OPA_LINE_STATUS_H
