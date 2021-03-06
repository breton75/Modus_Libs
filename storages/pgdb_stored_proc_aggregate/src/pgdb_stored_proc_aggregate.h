﻿#ifndef SV_PGDB_AGGREGATE_STORAGE_H
#define SV_PGDB_AGGREGATE_STORAGE_H

#include "pgdb_stored_proc_aggregate_global.h"

#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QHostAddress>
#include <QTime>
#include <QElapsedTimer>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../Modus/global/storage/sv_abstract_storage.h"
#include "../../../Modus/global/global_defs.h"

#include "../../../svlib/sv_pgdb.h"
#include "params.h"

extern "C" {

    PGDB_STORED_PROC_AGGREGATE_SHARED_EXPORT modus::SvAbstractStorage* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace pgsp {

  class pgStoredProcStorage;

}


class pgsp::pgStoredProcStorage: public modus::SvAbstractStorage
{
  Q_OBJECT

public:
  pgStoredProcStorage();
  ~pgStoredProcStorage();

  bool configure(modus::StorageConfig* config) Q_DECL_OVERRIDE;

private:
  pgsp::Params m_params;
  SvPGDB* PGDB = nullptr;

  QString m_last_error = "";

  QTimer* m_reconnect_timer = nullptr;

  bool connect();
  void processSignals() override;

private slots:
  void reconnect();
  void start_reconnect_timer();
  void stop_reconnect_timer();

//  void stopThread();
  void deleteThread();

  void logerr(QString e);
  void logreconnect();

};


#endif // SV_PGDB_AGGREGATE_STORAGE_H
