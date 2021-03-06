﻿#ifndef CONNING_KONGSBER_DEVICE_H
#define CONNING_KONGSBER_DEVICE_H

#include <QRegularExpression>
#include <QNetworkInterface>

#include "proj_12700_opa_global.h"

#include "../../../Modus/global/sv_abstract_device.h"
#include "../../../Modus/global/sv_signal.h"

#include "../../../svlib/sv_abstract_logger.h"
#include "../../../svlib/sv_exception.h"
#include "../../../svlib/sv_crc.h"

#include "device_params.h"
#include "ifc_serial_params.h"
#include "ifc_udp_params.h"
//#include "ifc_test_params.h"
#include "signal_params.h"
#include "opa_defs.h"

#include "collection_0x02.h"
#include "collection_0x03.h"
#include "collection_0x04.h"
#include "collection_0x33.h"
#include "collection_0x19.h"
#include "collection_status.h"

extern "C" {

    GAMMA_OPASHARED_EXPORT ad::SvAbstractDevice* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace opa {

  #pragma pack(push,1)
  struct Header
  {
    quint8  client_addr;
    quint8  func_code;
    quint8  ADDRESS;
    quint8  OFFSET;
    quint16 register_count;
    quint8  byte_count;
  };
  #pragma pack(pop)

  class SvOPA;
  class GenericThread;
  class UDPThread;
  class SerialThread;

  typedef QMap<int, SvAbstractSignalCollection*> SignalCollections;

  bool parse_signal(SvSignal* signal);

  enum AvailableIfces {
    Undefined = -1,
    RS,
    RS485,
    UDP
  };

  const QMap<QString, AvailableIfces> ifcesMap = {{"RS",    AvailableIfces::RS},
                                                  {"RS485", AvailableIfces::RS485},
                                                  {"UDP",   AvailableIfces::UDP}};

//  const QMap<quint8, QString> datatypeByTag = {{0x02, "0x02"},
//                                               {0x03, "0x03"},
//                                               {0x04, "0x04"},
//                                               {0x33, "0x33"},
//                                               {0x19, "0x19"}};

}


class opa::SvOPA: public ad::SvAbstractDevice
{
  Q_OBJECT

public:
  SvOPA(sv::SvAbstractLogger* logger = nullptr);
  ~SvOPA();

  bool configure(const ad::DeviceConfig& cfg);

  bool open();
  void close();

  void create_new_thread() throw(SvException);

  void addSignal(SvSignal* signal) throw(SvException) ;//override;

//  DeviceParams* params() { return &_params; }

private:


  bool is_configured = false;

private slots:
  void deleteThread();

signals:
  void stopThread();

};

class opa::GenericThread: public ad::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  GenericThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr):
    ad::SvAbstractDeviceThread(device, logger)
  {
    me = sv::log::sender(device->config()->name);
  }

  ~GenericThread();

  void initSignalsCollections() throw(SvException);

protected:
  opa::DeviceParams dev_params;

  opa::DATA m_data;

  opa::Header m_header;
  size_t m_hsz = sizeof(opa::Header);

  sv::log::sender me;

  opa::Type0x02   type0x02_signals;
  opa::Type0x03   type0x03_signals;
  opa::Type0x04   type0x04_signals;
  opa::Type0x19   type0x19_signals;
  opa::Type0x33   type0x33_signals;
  opa::LineStatus line_status_signals;

  opa::SignalCollections signal_collections;

  void process_data();

private:
  quint16 parse_data(ad::BUFF* buff, opa::DATA* data, opa::Header* header);

//  void func_reset(ad::SvAbstractDevice* device);
//  void func_0x77(ad::SvAbstractDevice* device);
//  void func_0x02(ad::SvAbstractDevice* device, ad::DATA* data);
//  void func_0x03(ad::SvAbstractDevice* device, ad::DATA* data);
//  void func_0x04(ad::SvAbstractDevice* device, ad::DATA* data);
//  void func_0x19(ad::SvAbstractDevice* device, ad::DATA* data);



//  void func_set_line_status(ad::SvAbstractDevice* device, ad::DATA* data);

//  QString getPrefix(quint16 start_register);

//  void parseNlog(const QString &message);
//  void parse_GEN(const QString& message);
//  void parse_XDR(const QString& message);

  QByteArray confirmation();


};

class opa::UDPThread: public opa::GenericThread
{
  Q_OBJECT

public:
  UDPThread(ad::SvAbstractDevice *device, sv::SvAbstractLogger* logger = nullptr);
  ~UDPThread();

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

  void conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException);

private:
  QUdpSocket socket;

  UdpParams    ifc_params;

  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};

class opa::SerialThread: public opa::GenericThread
{
  Q_OBJECT

public:
  SerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr);
  ~SerialThread();

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

  void conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException);

private:
  QSerialPort port;

  SerialParams ifc_params;

  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};


#endif // CONNING_KONGSBER_DEVICE_H
