#ifndef SV_TCP_H
#define SV_TCP_H

#include <QNetworkInterface>
#include <QTcpSocket>

#include "ifc_tcp_global.h"
#include "tcp_defs.h"
//#include "sv_tcp_client.h"

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
  QTcpSocket*   m_client = nullptr;
//  QTcpSocket*   m_server = nullptr;
  tcp::Params   m_params;

  QTimer*        m_gap_timer;

  void datalog(const QByteArray& bytes, QString& message);

private slots:
  void socketError(QAbstractSocket::SocketError err);
  void stateChanged(QAbstractSocket::SocketState state);

  bool connectToServer();
  void newData();
  void emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type);

};

#endif // SV_TCP_H
