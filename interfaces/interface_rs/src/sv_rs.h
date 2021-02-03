#ifndef SV_RS_H
#define SV_RS_H

#include <QSerialPort>

#include "ifc_rs_global.h"
#include "rs_defs.h"

#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

extern "C" {

    IFC_RS_EXPORT modus::SvAbstractInterface* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

class SvRS: public modus::SvAbstractInterface
{
public:
  SvRS();

  virtual bool configure(const modus::DeviceConfig& config) override;

protected:
  virtual void run() override;

private:
  QSerialPort*  m_port    = nullptr;
  SerialParams  m_params;

  void write(modus::BUFF* buffer);

};

#endif // SVUDP_H
