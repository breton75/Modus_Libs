#ifndef SV_OPA_H
#define SV_OPA_H

#include <QObject>
#include <QUdpSocket>
#include <QSerialPort>

#include "proj_12700_opa_global.h"

#include "../../../Modus/global/sv_abstract_device.h"
#include "../../../Modus/global/sv_signal.h"

#include "../../../svlib/sv_abstract_logger.h"
#include "../../../svlib/sv_exception.h"
#include "../../../svlib/sv_crc.h"

#include "deviceparams.h"

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

  enum Ifces {
    RS485,
    UDP
  };

  const QMap<QString, Ifces> ifcesMap = {{"RS485", Ifces::RS485}, {"UDP", Ifces::UDP}};

  quint16 parse_data(ad::BUFF* buff, ad::DATA* data, opa::Header* header);
  QByteArray confirmation(const opa::Header* header);

  void func_reset(ad::SvAbstractDevice* device);
  void func_0x77(ad::SvAbstractDevice* device);
  void func_0x02(ad::SvAbstractDevice* device, ad::DATA* data);
  void func_0x03(ad::SvAbstractDevice* device, ad::DATA* data);
  void func_0x04(ad::SvAbstractDevice* device, ad::DATA* data);
  void func_0x19(ad::SvAbstractDevice* device, ad::DATA* data);
  void func_set_line_status(ad::SvAbstractDevice* device, ad::DATA* data);

  inline QString getPrefix(quint16 start_register);

}


class opa::SvOPA: public ad::SvAbstractDevice
{

public:
  SvOPA(sv::SvAbstractLogger *logger = nullptr);
};

class opa::GenericThread: public ad::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  GenericThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr):
    ad::SvAbstractDeviceThread(device, logger)
  {  }

protected:
  opa::DeviceParams dev_params;

  size_t hsz = sizeof(ckng::Header);

  void process_data();
//  void process_signals();

private:
//  QString m_current_message;
  QRegularExpression m_re_full;
  QRegularExpression m_re_XDR;
  QRegularExpression m_re_GEN;
  QRegularExpression m_re_header;
  QRegularExpression m_re_tail; // = QRegularExpression("/wds/");

  QString m_head;

  void parseNlog(const QString &message);
  void parse_GEN(const QString& message);
  void parse_XDR(const QString& message);

  QByteArray confirmation();


};

class opa::UDPThread: public opa::GenericThread
{
  Q_OBJECT

public:
  UDPThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr);
  ~UDPThread();

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

  void conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException);

private:
  sv::log::sender me;

  QUdpSocket m_socket;

  UdpParams    m_ifc_params;

  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};

class opa::SerialThread: public opa::GenericThread
{
  Q_OBJECT

public:
  SerialThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger *logger = nullptr);
  ~SerialThread();

  void open() throw(SvException);

  quint64 write(const QByteArray& data);

  void conform(const QString& jsonDevParams, const QString& jsonIfcParams) throw(SvException);

private:
  sv::log::sender me;



  void run() Q_DECL_OVERRIDE;

public slots:
  void stop();

};

#endif // SV_OPA_H
