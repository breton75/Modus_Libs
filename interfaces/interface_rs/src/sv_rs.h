#ifndef SV_RS_H
#define SV_RS_H

#include <QSerialPort>

#include "ifc_rs_global.h"
#include "rs_defs.h"

#include "../../../../Modus_Libs/APAK/global_apak_defs.h"
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

private:
  QSerialPort*  m_port    = nullptr;
  SerialParams  m_params;

  QTimer*        m_gap_timer;

public slots:
  bool start() override;
  void read() override;
  void write(modus::BUFF* buffer) override;

private slots:
  void newData();
  void emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type);

};

#endif // SVUDP_H
