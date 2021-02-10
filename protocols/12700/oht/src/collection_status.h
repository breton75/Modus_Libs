#ifndef COLLECTION_OHT_LINE_STATUS_H
#define COLLECTION_OHT_LINE_STATUS_H

#include <QObject>
#include <QList>

#include "oht_defs.h"

namespace oht {

  class LineStatus : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit LineStatus();

    void addSignal(modus::SvSignal* signal); //throw (SvException);

    void updateSignals(const oht::DATA* data = nullptr);

  private:
    QList<modus::SvSignal*> m_signals;

  };
}


#endif // COLLECTION_OHT_LINE_STATUS_H
