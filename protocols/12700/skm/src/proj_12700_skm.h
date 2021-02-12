#ifndef PROJ_12700_SKM_H
#define PROJ_12700_SKM_H

#include <QMutex>
#include <QMutexLocker>

#include "proj_12700_skm_global.h"

#include "../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../Modus/global/signal/sv_signal.h"

#include "skm_params.h"
#include "signal_params.h"
#include "skm_defs.h"

#include "collection_0x02.h"
#include "collection_0x01.h"

#include "../../../svlib/sv_abstract_logger.h"
#include "../../../svlib/sv_exception.h"
#include "../../../svlib/sv_crc.h"

extern "C" {

    PROJ_12700_SKM_EXPORT modus::SvAbstractProtocol* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

#define GOOD_PARSED   0
#define DO_RESET      1
#define DO_NOT_RESET  2

namespace skm {

  #pragma pack(push,1)
  struct Header
  {
    quint8  begin_0x1F;
    quint8  DST;
    quint8  SRC;
    quint8  version;
  };
  #pragma pack(pop)

  struct PARSERESULT {

    PARSERESULT();
    PARSERESULT(bool reset, QDateTime dt = QDateTime())
    { do_reset = reset; parse_time = dt; }

    bool do_reset;
    QDateTime parse_time;

  };

  class SvSKM;

  typedef QMap<int, SvAbstractSignalCollection*> SignalCollections;

//  bool parse_signal(modus::SvSignal* signal);

}

class skm::SvSKM: public modus::SvAbstractProtocol
{
  Q_OBJECT

public:
  SvSKM();

  bool configure(modus::DeviceConfig* config, modus::IOBuffer *iobuffer) override;

protected:
  void run() override;

  void disposeInputSignal (modus::SvSignal* signal) override;
  void disposeOutputSignal (modus::SvSignal* signal) override;

//  void validateSignals(QDateTime& lastParsedTime) override;

private:
  QList<modus::SvSignal*>   p_input_signals;
  QList<modus::SvSignal*>   p_output_signals;

  skm::DeviceParams m_params;

  skm::DATA m_data;

  skm::Header m_header;
  size_t m_hsz = sizeof(skm::Header);

  skm::Type0x01   type0x01_signals;
  skm::Type0x02   type0x02_signals;

  skm::SignalCollections signal_collections;

  PARSERESULT parse();
  bool check_1F_2F_55(quint8 byte);
  quint16 pull_data(modus::BUFF* buff, skm::DATA* data, skm::Header* header);
  void confirmation();

};

#endif // PROJ_12700_SKM_H
