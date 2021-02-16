#ifndef SV_CAN_H
#define SV_CAN_H

#include <QSerialPort>

#include "ifc_can_global.h"
#include "can_defs.h"

#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

extern "C" {

    IFC_CAN_EXPORT modus::SvAbstractInterface* create();

    IFC_CAN_EXPORT const char* getDefaultParams();
    IFC_CAN_EXPORT const char* getName();
    IFC_CAN_EXPORT const char* getDescription();
}

class SvCAN: public modus::SvAbstractInterface
{
public:
  SvCAN();

  virtual bool configure(const modus::DeviceConfig& config) override;

protected:
  virtual void run() override;

private:
  QSerialPort*  m_port    = nullptr;
  SerialParams  m_params;

  void write(modus::BUFF* buffer);

};

#endif // SVUDP_H
