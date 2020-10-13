#ifndef COLLECTION_OPA_LINE_STATUS_H
#define COLLECTION_OPA_LINE_STATUS_H

#include <QObject>
#include <QList>

#include "../../../Modus/global/sv_abstract_signal_collection.h"

namespace opa {

  class LineStatus : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit LineStatus();

    void addSignal(SvSignal* signal) throw (SvException);

    void updateSignals(const ad::DATA* data = nullptr);

  private:
    QList<SvSignal*> m_signals;

  };
}


#endif // COLLECTION_OPA_LINE_STATUS_H
