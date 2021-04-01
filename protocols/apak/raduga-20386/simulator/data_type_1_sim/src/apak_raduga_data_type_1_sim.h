#ifndef APAK_RADUGA_DATATYPE1_H
#define APAK_RADUGA_DATATYPE1_H

#include <QMutex>
#include <QMutexLocker>

#include "apak_raduga_data_type_1_sim_global.h"

#include "../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../Modus/global/signal/sv_signal.h"

#include "protocol_params.h"
#include "raduga_defs.h"

#include "collection_0x02.h"
#include "collection_0x03.h"
#include "collection_data_type_1.h"
#include "collection_0x33.h"
#include "collection_0x19.h"
#include "collection_status.h"

#include "../../../../../svlib/sv_abstract_logger.h"
#include "../../../../../svlib/sv_exception.h"
#include "../../../../../svlib/sv_crc.h"

extern "C" {

    APAK_RADUGA_DATATYPE1_EXPORT modus::SvAbstractProtocol* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

#define GOOD_PARSED   0
#define DO_RESET      1
#define DO_NOT_RESET  2

namespace raduga {

  #pragma pack(push,1)
  struct Header
  {
    char    system_name[16];
    quint16 abonent_id;
    quint16 activity_id;
    quint16 data_type;
    char    reserv[10];
  };
  #pragma pack(pop)

  struct PARSERESULT {

    PARSERESULT();
    PARSERESULT(bool reset, QDateTime dt = QDateTime())
    { do_reset = reset; parse_time = dt; }

    bool do_reset;
    QDateTime parse_time;

  };

  class SvRaduga;

  typedef QMap<int, SvAbstractSignalCollection*> SignalCollections;

//  bool parse_signal(modus::SvSignal* signal);

}

class raduga::SvRaduga: public modus::SvAbstractProtocol
{
  Q_OBJECT

public:
  SvRaduga();

  bool configure(modus::DeviceConfig* config, modus::IOBuffer *iobuffer) override;

protected:
  void run() override;

  void disposeInputSignal (modus::SvSignal* signal) override;
  void disposeOutputSignal (modus::SvSignal* signal) override;

//  void validateSignals(QDateTime& lastParsedTime) override;

private:
  QList<modus::SvSignal*>   p_input_signals;
  QList<modus::SvSignal*>   p_output_signals;

  raduga::DeviceParams m_params;

  raduga::DATA m_data;

  raduga::Header m_header;
  size_t m_hsz = sizeof(opa::Header);

  opa::Type0x02   type0x02_signals;
  opa::Type0x03   type0x03_signals;
  opa::Type0x04   type0x04_signals;
  opa::Type0x19   type0x19_signals;
  opa::Type0x33   type0x33_signals;
  opa::LineStatus line_status_signals;

  raduga::SignalCollections signal_collections;

  PARSERESULT parse();
  void confirmation();

};

#endif // APAK_RADUGA_DATATYPE1_H
