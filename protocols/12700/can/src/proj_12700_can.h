#ifndef PROJ_12700_CAN_H
#define PROJ_12700_CAN_H

#include "linux/can.h"

#include <QMutex>
#include <QMutexLocker>

#include "proj_12700_can_global.h"

#include "../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../Modus/global/signal/sv_signal.h"

//#include "can_params.h"
#include "can_defs.h"
#include "can12700_signal.h"

#include "../../../../../Modus/global/misc/sv_abstract_logger.h"
#include "../../../../../Modus/global/misc/sv_exception.h"
#include "../../../../../svlib/sv_crc.h"

extern "C" {

    PROJ_12700_CAN_EXPORT modus::SvAbstractProtocol* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

#define GOOD_PARSED   0
#define DO_RESET      1
#define DO_NOT_RESET  2

namespace can {

//  #pragma pack(push,1)
//  struct Header
//  {
//    quint8  begin_0x1F;
//    quint8  DST;
//    quint8  SRC;
//    quint8  version;
//  };
//  #pragma pack(pop)

  struct PARSERESULT {

    PARSERESULT();
    PARSERESULT(bool reset, QDateTime dt = QDateTime())
    { do_reset = reset; parse_time = dt; }

    bool do_reset;
    QDateTime parse_time;

  };

  class SvCAN12700;

//  typedef QMap<int, modus::SvAbstractSignalCollection*> SignalCollections;

//  bool parse_signal(modus::SvSignal* signal);

}

class can::SvCAN12700: public modus::SvAbstractProtocol
{
  Q_OBJECT

public:
  SvCAN12700();

  bool configure(modus::DeviceConfig* config, modus::IOBuffer *iobuffer) override;

protected:
  void run() override;

  void disposeInputSignal (modus::SvSignal* signal) override;
  void disposeOutputSignal (modus::SvSignal* signal) override;

//  void validateSignals(QDateTime& lastParsedTime) override;

private:
  QList<modus::SvSignal*>   p_input_signals;
  QList<modus::SvSignal*>   p_output_signals;

//  can::DeviceParams m_params;

  can::DATA m_data;

//  skm::Header m_header;
  size_t m_hsz = sizeof(struct canfd_frame) - 8;

  can::CANSignalCollection signal_collection;

  can::PARSERESULT parse();
//  quint16 pull_data(modus::BUFF* buff, skm::DATA* data, skm::Header* header);
//  void confirmation();

};

#endif // PROJ_12700_CAN_H
