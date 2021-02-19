#ifndef SV_CAN_H
#define SV_CAN_H

#include <QProcess>

#include <linux/can.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//#include <string.h>

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
  ~SvCAN();

  virtual bool configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer) override;

protected:
  virtual void run() override;

private:
  CANParams     m_params;

  int sock = 0;
  struct sockaddr_can addr;
  struct can_frame frame;
  struct ifreq ifr;

};

#endif // SVUDP_H
