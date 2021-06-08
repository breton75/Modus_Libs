﻿#ifndef DATA_TYPE_COLLECTION_H
#define DATA_TYPE_COLLECTION_H

#include <QObject>
#include <QList>

#include "raduga_defs.h"

namespace raduga {

  struct SignalStruct {

    SignalStruct(modus::SvSignal* signal, raduga::SignalParams params):
      signal(signal), params(params)
    {  }

    modus::SvSignal*      signal;
    raduga::SignalParams  params;

  };

  class DataTypeCollection : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit DataTypeCollection();

    void addSignal(modus::SvSignal* signal, quint16 bufsize); // throw (SvException);

    void updateSignals(const char* data, quint16 len);

    void updateOutput(const modus::BUFF* data = nullptr);

  private:
    QList<SignalStruct> m_signals;

  };
}


#endif // DATA_TYPE_COLLECTION_H
