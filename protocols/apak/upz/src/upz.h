﻿#ifndef UPZ_H
#define UPZ_H

#include <QMutex>
#include <QMutexLocker>

#include "upz_global.h"

#include "../../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../../Modus/global/signal/sv_signal.h"

#include "protocol_params.h"

#include "../../../../../../svlib/SvAbstractLogger/1.2/sv_abstract_logger.h"
#include "../../../../../../svlib/SvException/1.1/sv_exception.h"
#include "../../../../../../svlib/SvCRC/1.0/sv_crc.h"

extern "C" {

    UPZ_EXPORT modus::SvAbstractProtocol* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace apak {

  class SvUPZ: public modus::SvAbstractProtocol
  {
    Q_OBJECT

  public:
    SvUPZ();
    ~SvUPZ();

    bool configure(modus::DeviceConfig* config, modus::IOBuffer *iobuffer) override;

  protected:
    void run() override;

    void disposeInputSignal (modus::SvSignal* signal) override;
    void disposeOutputSignal(modus::SvSignal* signal) override;

  private:
    apak::ProtocolParams  m_params;

    modus::SvSignal*      m_signal;

  };
}


#endif // UNIVERSAL_PACK_H