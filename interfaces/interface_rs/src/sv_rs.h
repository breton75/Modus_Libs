#ifndef SV_RS_H
#define SV_RS_H

#include <QSerialPort>

#include "ifc_rs_global.h"
#include "rs_defs.h"

#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

extern "C" {

    IFC_RS_EXPORT modus::SvAbstractInterface* create();

    IFC_RS_EXPORT const char* getDefaultParams();
    IFC_RS_EXPORT const char* getName();
    IFC_RS_EXPORT const char* getDescription();
}

class SvRS: public modus::SvAbstractInterface
{
public:
  SvRS();

  virtual bool configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer) override;

protected:
  virtual void run() override;

private:
  QSerialPort*  m_port    = nullptr;
  SerialParams  m_params;

  void write(modus::BUFF* buffer);

};

#endif // SVUDP_H
