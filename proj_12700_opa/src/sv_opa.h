#ifndef SV_OPA_H
#define SV_OPA_H

#include "proj_12700_opa_global.h"

#include "../../../Modus/global/sv_abstract_device.h"
#include "../../../Modus/global/sv_signal.h"

#include "../../../svlib/sv_abstract_logger.h"
#include "../../../svlib/sv_exception.h"
#include "../../../svlib/sv_crc.h"

extern "C" {

    GAMMA_OPASHARED_EXPORT ad::SvAbstractDevice* create();

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

  class SvUDPThread;
  class SvSerialThread;

  enum Ifces {
    RS485,
    UDP
  };

  const QMap<QString, Ifces> ifcesMap = {{"RS485", Ifces::RS485}, {"UDP", Ifces::UDP}};

  quint16 parse_data(dev::BUFF* buff, dev::DATA* data, opa::Header* header);
  QByteArray confirmation(const opa::Header* header);

  void func_reset(dev::SvAbstractDevice* device);
  void func_0x77(dev::SvAbstractDevice* device);
  void func_0x02(dev::SvAbstractDevice* device, dev::DATA* data);
  void func_0x03(dev::SvAbstractDevice* device, dev::DATA* data);
  void func_0x04(dev::SvAbstractDevice* device, dev::DATA* data);
  void func_0x19(dev::SvAbstractDevice* device, dev::DATA* data);
  void func_set_line_status(dev::SvAbstractDevice* device, dev::DATA* data);

  inline QString getPrefix(quint16 start_register);

}

class SvOPA: public ad::SvAbstractDevice
{

public:
  SvOPA(sv::SvAbstractLogger *logger = nullptr);
};

#endif // SV_OPA_H
