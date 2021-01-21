#ifndef PROJ_12700_OPA_MOD_H
#define PROJ_12700_OPA_MOD_H

#include <QMutex>
#include <QMutexLocker>

#include "proj_12700_opa_mod_global.h"

#include "../../../Modus/global/device/sv_abstract_protocol.h"
#include "../../../Modus/global/signal/sv_signal.h"

#include "opa_params.h"
#include "signal_params.h"
#include "opa_defs.h"

#include "collection_0x02.h"
#include "collection_0x03.h"
#include "collection_0x04.h"
#include "collection_0x33.h"
#include "collection_0x19.h"
#include "collection_status.h"

#include "../../../svlib/sv_abstract_logger.h"
#include "../../../svlib/sv_exception.h"
#include "../../../svlib/sv_crc.h"

extern "C" {

    PROJ_12700_OPA_MOD_EXPORT modus::SvAbstractProtocol* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace opa {

  #pragma pack(push,1)
  struct Header
  {
    quint8  client_addr;
    quint8  func_code;
    quint8  ADDRESS;
    quint8  OFFSET;
    quint16 register_count;
    quint8  byte_count;
  };
  #pragma pack(pop)

  class SvOPA;

  typedef QMap<int, SvAbstractSignalCollection*> SignalCollections;

//  bool parse_signal(modus::SvSignal* signal);

}

class opa::SvOPA: public modus::SvAbstractProtocol
{
public:
  SvOPA();

  bool configure(const modus::DeviceConfig& cfg);

protected:
  void disposeSignal (modus::SvSignal* signal);

  bool parse_input_data();
  bool form_output_data();

private:
  opa::DeviceParams m_dev_params;

  opa::DATA m_data;

  opa::Header m_header;
  size_t m_hsz = sizeof(opa::Header);

  opa::Type0x02   type0x02_signals;
  opa::Type0x03   type0x03_signals;
  opa::Type0x04   type0x04_signals;
  opa::Type0x19   type0x19_signals;
  opa::Type0x33   type0x33_signals;
  opa::LineStatus line_status_signals;

  opa::SignalCollections signal_collections;

  void confirmation();


};

#endif // PROJ_12700_OPA_MOD_H
