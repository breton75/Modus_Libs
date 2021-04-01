#ifndef DATA_TYPE_1_H
#define DATA_TYPE_1_H

#include <QObject>
#include <QMap>
#include <QMultiMap>

#include "raduga_defs.h"

namespace raduga {

  class Type0x02 : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit Type0x02();

    void addSignal(modus::SvSignal* signal); // throw (SvException);

    void updateSignals(const opa::DATA* data = nullptr);

  private:
    QMultiMap<quint32, modus::SvSignal*> m_signals;

  };
}


#endif // DATA_TYPE_1_H
