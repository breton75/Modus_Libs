#ifndef SV_TCP_H
#define SV_TCP_H

#include <QNetworkInterface>
#include <QTcpSocket>

#include "ifc_tcp_global.h"
#include "tcp_defs.h"

#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

extern "C" {

    IFC_TCP_EXPORT modus::SvAbstractInterface* create();

    IFC_TCP_EXPORT const char* getDefaultParams();
    IFC_TCP_EXPORT const char* getName();
    IFC_TCP_EXPORT const char* getDescription();
}

class SvTcp: public modus::SvAbstractInterface
{
public:
  SvTcp();

  virtual bool configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer) override;

public slots:
  bool start() override;
  void read() override;
  void write(modus::BUFF* buffer) override;

private:
  QTcpSocket*   m_socket = nullptr;
  TcpParams     m_params;


//private slots:
//  void read();

};

#endif // SV_TCP_H
