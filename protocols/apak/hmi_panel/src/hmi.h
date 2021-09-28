#ifndef HMI_H
#define HMI_H

#include <QMutex>
#include <QMutexLocker>

#include "hmi_global.h"

#include "../../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../../Modus/global/signal/sv_signal.h"

#include "protocol_params.h"

#include "../../../../../../svlib/SvAbstractLogger/1.2/sv_abstract_logger.h"
#include "../../../../../../svlib/SvException/1.1/sv_exception.h"
#include "../../../../../../svlib/SvCRC/1.0/sv_crc.h"

extern "C" {

    HMI_EXPORT modus::SvAbstractProtocol* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace apak {

  class SvHMI: public modus::SvAbstractProtocol
  {
    Q_OBJECT

  public:
    SvHMI();
    ~SvHMI();

    bool configure(modus::DeviceConfig* config, modus::IOBuffer *iobuffer) override;

    bool bindSignal(modus::SvSignal *signal, modus::SignalBinding binding) override;

  public slots:
    void start() override;

    void signalUpdated(modus::SvSignal* signal) override;
    void signalChanged(modus::SvSignal* signal) override;

  private:
    apak::ProtocolParams                m_params;

    QMap<QString, modus::SvSignal*>     m_input_signals;
    QMap<QString, modus::SvSignal*>     m_output_signals;

  };
}


#endif // UNIVERSAL_PACK_H
