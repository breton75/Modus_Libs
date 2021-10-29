#ifndef SV_UDP_H
#define SV_UDP_H

#include <QNetworkInterface>
#include <QUdpSocket>

#include "ifc_udp_global.h"
#include "udp_defs.h"

#include "../../../../Modus_Libs/APAK/global_apak_defs.h"
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

public slots:
  bool start() override;
  void read() override;
  void write(modus::BUFF* buffer) override;

private:
  QUdpSocket*   m_socket = nullptr;
  UdpParams     m_params;

  QTimer*       m_gap_timer;

private slots:
  void newData();
  void emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type);


};

#endif // SV_UDP_H
