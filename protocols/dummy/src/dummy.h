#ifndef DUMMY_PRTCL_H
#define DUMMY_PRTCL_H

#include <QMutex>
#include <QMutexLocker>

#include "dummy_global.h"
#include "protocol_params.h"

#include "../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../Modus/global/signal/sv_signal.h"

#include "../../../../svlib/SvAbstractLogger/1.2/sv_abstract_logger.h"
#include "../../../../svlib/SvException/1.1/sv_exception.h"
#include "../../../../svlib/SvCRC/1.1/sv_crc.h"

extern "C" {

    SKM_IMITATOR_EXPORT modus::SvAbstractProtocol* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace plug {

  class SvDummy: public modus::SvAbstractProtocol
  {
    Q_OBJECT

  public:
    SvDummy();
    ~SvDummy();

    bool configure(modus::DeviceConfig* config, modus::IOBuffer *iobuffer) override;
    bool bindSignal(modus::SvSignal* signal, modus::SignalBinding binding) override;

  private:
    plug::ProtocolParams  m_params;

//    QMap<quint8, QList<modus::SvSignal*>>     m_signals_by_vin;
    QMap<modus::SvSignal*, plug::SignalParams> m_params_by_signal;

    QByteArray send_data;

    QTimer* m_timer;

  public slots:
    void signalUpdated(modus::SvSignal* signal) override;
    void signalChanged(modus::SvSignal* signal) override;
    void start() override;

  private slots:
    void parse(modus::BUFF* buffer);

  };
}


#endif // DUMMY_PRTCL_H
