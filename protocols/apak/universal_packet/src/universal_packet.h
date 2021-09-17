#ifndef UNIVERSAL_PACK_H
#define UNIVERSAL_PACK_H

#include <QMutex>
#include <QMutexLocker>

#include "universal_packet_global.h"

#include "../../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../../Modus/global/signal/sv_signal.h"

#include "protocol_params.h"

#include "../../../../../../svlib/SvAbstractLogger/1.2/sv_abstract_logger.h"
#include "../../../../../../svlib/SvException/1.1/sv_exception.h"
#include "../../../../../../svlib/SvCRC/1.0/sv_crc.h"

extern "C" {

    UNIVERSAL_PACKET_EXPORT modus::SvAbstractProtocol* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace apak {

  class SvUniversalPack: public modus::SvAbstractProtocol
  {
    Q_OBJECT

  public:
    SvUniversalPack();
    ~SvUniversalPack();

    bool configure(modus::DeviceConfig* config, modus::IOBuffer *iobuffer) override;

  protected:
    void run() override;

//    void disposeInputSignal (modus::SvSignal* signal) override;
//    void disposeOutputSignal(modus::SvSignal* signal) override;

    void disposeSignal(modus::SvSignal* signal) override;

  private:
    apak::ProtocolParams  m_params;

    modus::SvSignal*      m_in_signal;
    modus::SvSignal*      m_job_signal;

  };
}


#endif // UNIVERSAL_PACK_H
