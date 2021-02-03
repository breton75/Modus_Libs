#ifndef SVUDP_H
#define SVUDP_H

#include <QUdpSocket>

#include "ifc_udp_global.h"
#include "udp_defs.h"

#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"
#include "../../../../Modus/global/signal/sv_signal.h"

extern "C" {

    IFC_UDP_EXPORT modus::SvAbstractInterface* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

class SvUdp: public modus::SvAbstractInterface
{
public:
  SvUdp();

  virtual bool configure(const modus::DeviceConfig& config) override;

protected:
  virtual void run() override;

private:
  QUdpSocket*   m_socket = nullptr;
  UdpParams     m_params;

  void write(modus::BUFF* buffer);

};

#endif // SVUDP_H
