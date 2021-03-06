﻿#ifndef SV_UDP_H
#define SV_UDP_H

#include <QNetworkInterface>
#include <QUdpSocket>

#include "ifc_udp_global.h"
#include "udp_defs.h"

#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

extern "C" {

    IFC_UDP_EXPORT modus::SvAbstractInterface* create();

    IFC_UDP_EXPORT const char* getDefaultParams();
    IFC_UDP_EXPORT const char* getName();
    IFC_UDP_EXPORT const char* getDescription();
}

class SvUdp: public modus::SvAbstractInterface
{
public:
  SvUdp();

  virtual bool configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer) override;

protected:
  virtual void run() override;

private:
  QUdpSocket*   m_socket = nullptr;
  UdpParams     m_params;

  void write(modus::BUFF* buffer);

};

#endif // SV_UDP_H
