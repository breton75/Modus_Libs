#ifndef CONNING_KONGSBER_DEVICE_H
#define CONNING_KONGSBER_DEVICE_H

#include <QRegularExpression>
#include <QNetworkInterface>
//#include <QVariantMap>

#include "interserver_exchange_receiver_global.h"

#include "../../../../Modus/global/sv_abstract_device.h"
#include "../../../../Modus/global/sv_signal.h"

#include "../../../../svlib/sv_abstract_logger.h"
#include "../../../../svlib/sv_exception.h"
#include "../../../../svlib/sv_crc.h"

#include "../../global/ise_defs.h"

#include "device_params.h"
#include "ifc_udp_params.h"
//#include "ifc_test_params.h"
#include "signal_params.h"


extern "C" {

    ISER_SHARED_EXPORT ad::SvAbstractDevice* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace iser {

  #pragma pack(push,1)
  struct Header
  {
    char    sign[3];
    quint16 sender;
    quint16 receiver;
    quint16 data_length;
  };
  #pragma pack(pop)

  class SvISER;
  class GenericThread;
  class UDPThread;

  bool parse_signal(SvSignal* signal);

  enum AvailableIfces {
    Undefined = -1,
    UDP
  };

  const QMap<QString, AvailableIfces> ifcesMap = {{"UDP",   AvailableIfces::UDP}};

}


class iser::SvISER: public ad::SvAbstractDevice
{
  Q_OBJECT

public:
  SvISER(sv::SvAbstractLogger* logger = nullptr);
  ~SvISER();

  bool configure(const ad::DeviceConfig& cfg);

  bool open();
  void close();

  void create_new_thread() throw(SvException);

private:
  bool is_configured = false;

private slots:
  void deleteThread();

signals:
  void stopThread();

};

class iser::GenericThread: public ad::SvAbstractDeviceThread
{
  Q_OBJECT

public:
  GenericThread(ad::SvAbstractDevice* device, sv::SvAbstractLogger* logger = nullptr):
    ad::SvAbstractDeviceThread(device, logger)
  {
    me = sv::log::sender(device->config()->name);
  }

  ~GenericThread();

protected:
  iser::DeviceParams dev_params;

  iser::Header m_header;
  size_t m_hsz = sizeof(iser::Header);

  char m_def_sign[3] = {'I','S','E'};

  sv::log::sender me;

  void process_data();

private:
//  ise::DATA m_data;

//  quint16 parse_data(ad::BUFF* buff, ad::DATA* data, iser::Header* header);

  QByteArray confirmation();


};

class iser::UDPThread: public iser::GenericThread
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


#endif // CONNING_KONGSBER_DEVICE_H
