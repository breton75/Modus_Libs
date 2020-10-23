#ifndef ISES_DEVICE_H
#define ISES_DEVICE_H

#include "interserver_exchange_sender_global.h"

#include <QObject>
#include <QThread>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QHostAddress>
#include <QTime>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../svlib/sv_crc.h"
#include "../../../../Modus/global/sv_abstract_storage.h"
#include "../../global/ise_defs.h"

#include "storage_params.h"


extern "C" {

    ISES_SHARED_EXPORT as::SvAbstractStorage* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace ises {

  class SvISES;
  class SvISESThread;

}


class ises::SvISES: public as::SvAbstractStorage
{
  Q_OBJECT

public:
  SvISES(sv::SvAbstractLogger *logger = nullptr);
  ~SvISES();

  bool configure(const as::StorageConfig& config) Q_DECL_OVERRIDE;

  bool init()  Q_DECL_OVERRIDE;

  void start() Q_DECL_OVERRIDE;
  void stop()  Q_DECL_OVERRIDE;

  const ises::StorageParams* params() const { return &m_params; }

private:
  ises::StorageParams m_params;
  QString m_last_error = "";

  QTimer* m_reconnect_timer = nullptr;

private slots:
  void deleteThread();

  void logerr(QString e);
  void logreconnect();

};


class ises::SvISESThread: public as::SvAbstractStorageThread
{
    Q_OBJECT

public:
  SvISESThread(as::SvAbstractStorage* storage);
  ~SvISESThread();

  bool init();

  void run() Q_DECL_OVERRIDE;
  void stop();

private:

  ises::StorageParams m_params;

signals:
  void error(QString e);
  void connected();

};



#endif // ISES_DEVICE_H
