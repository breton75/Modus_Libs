#include "pgdb_stored_proc_aggregate.h"


pgspa::pgStoredProcAggStorage::pgStoredProcAggStorage(sv::SvAbstractLogger* logger):
  as::SvAbstractStorage(logger)
{
  p_signals.clear();

  m_reconnect_timer = new QTimer();
  m_reconnect_timer->setInterval(1000);

}

pgspa::pgStoredProcAggStorage::~pgStoredProcAggStorage()
{
//  stop();
//  if(_thr)
//    delete _thr;

//  _thr->stop();

//  while(_thr)
//    QCoreApplication::processEvents();

//  /** при завершении работы сервера, сбрасываем все сигналы **/
//  foreach (SvSignal* signal, _signals)
//    signal->setLostValue();

  deleteLater();

}

bool pgspa::pgStoredProcAggStorage::configure(const as::StorageConfig& config)
{
  p_config = config;

  try {

    /* парсим - проверяем, что парметры заданы верно */
    m_params = pgspa::StorageParams::fromJson(p_config.params);

    p_is_configured = true;

    return true;

  }

  catch(SvException& e) {

    p_is_configured = false;
    p_last_error = e.error;

    return false;
  }
}

bool pgspa::pgStoredProcAggStorage::init()
{
  try {

    p_thread = new pgspa::pgStoredProcAggThread(this);

    if(!p_thread->init())
      throw SvException(p_thread->lastError());

    return true;

  }

  catch(SvException& e) {

    p_last_error = e.error;

    delete p_thread;
    p_thread = nullptr;

    // запускаем реконнект
    if(!m_reconnect_timer->isActive()) // обязательно проверяем!
      start_reconnect_timer();

    logerr(p_last_error);

    return false;

  }
}

void pgspa::pgStoredProcAggStorage::logerr(QString e)
{
  *p_logger << sv::log::TimeZZZ << sv::log::llError << e << '\n' << sv::log::endl;
}

void pgspa::pgStoredProcAggStorage::logreconnect()
{
  if(p_logger)
    *p_logger << sv::log::TimeZZZ << sv::log::llInfo << QString("Фух! Восстановлена связь с хранилищем %1 [%2:%3:%4]\n")
                    .arg(p_config.name).arg(m_params.db)
                    .arg(m_params.host).arg(m_params.port)
              << sv::log::endl;
}

void pgspa::pgStoredProcAggStorage::reconnect()
{
  if(init()) {

    stop_reconnect_timer();
    start();

    logreconnect();

  }
}

void pgspa::pgStoredProcAggStorage::start_reconnect_timer()
{
  m_reconnect_timer->start();
  connect(m_reconnect_timer, &QTimer::timeout, this, &pgspa::pgStoredProcAggStorage::reconnect);
}

void pgspa::pgStoredProcAggStorage::stop_reconnect_timer()
{
  m_reconnect_timer->stop();
  disconnect(m_reconnect_timer, &QTimer::timeout, this, &pgspa::pgStoredProcAggStorage::reconnect);
}

void pgspa::pgStoredProcAggStorage::stop()
{
  if(!p_thread)
    return;

  /// при ручном завершении, отключаем переподключение
  disconnect(p_thread, &pgspa::pgStoredProcAggThread::finished, this, &pgspa::pgStoredProcAggStorage::start_reconnect_timer);
  disconnect((pgspa::pgStoredProcAggThread*)p_thread, &pgspa::pgStoredProcAggThread::connected, this, &pgspa::pgStoredProcAggStorage::logreconnect);

  stop_reconnect_timer();
  delete m_reconnect_timer;

  p_thread->stop();

}

void pgspa::pgStoredProcAggStorage::start()
{
  if(!p_thread)
    return;

//  connect(_thr, &pgspa::pgStoredProcAggThread::finished, _thr, &pgspa::pgStoredProcAggThread::deleteLater);
  connect((pgspa::pgStoredProcAggThread*)p_thread, &pgspa::pgStoredProcAggThread::finished, this, &pgspa::pgStoredProcAggStorage::deleteThread);
//  connect(_thr, &pgspa::pgStoredProcAggThread::finished, [=](){ delete _thr; _thr = nullptr; } );
  connect((pgspa::pgStoredProcAggThread*)p_thread, &pgspa::pgStoredProcAggThread::finished, this, &pgspa::pgStoredProcAggStorage::start_reconnect_timer);
  connect((pgspa::pgStoredProcAggThread*)p_thread, &pgspa::pgStoredProcAggThread::error, this, &pgspa::pgStoredProcAggStorage::logerr);

  p_thread->start();

}

//void pgspa::pgStoredProcAggStorage::stopThread()
//{
//  _thr->stop();
//}

void pgspa::pgStoredProcAggStorage::deleteThread()
{
  delete p_thread;
  p_thread = nullptr;
}

/** *************  Storage Thread  ******************* */

pgspa::pgStoredProcAggThread::pgStoredProcAggThread(as::SvAbstractStorage* storage):
  as::SvAbstractStorageThread(storage)
{

}

bool pgspa::pgStoredProcAggThread::init()
{
  try {

    m_params = pgspa::StorageParams::fromJson(p_storage->config()->params);


    PGDB = new SvPGDB();
    PGDB->setConnectionParams(m_params.db, m_params.host, m_params.port,
                              m_params.login, m_params.pass, m_params.role);

    QSqlError err = PGDB->connectToDB(QString("PGConn_%1").arg(p_storage->config()->id));

    if(err.type() != QSqlError::NoError)
      throw SvException(err.text());

    return true;

  }

  catch(SvException& e) {

    p_last_error = e.error;
    emit error(e.error);

    return false;

  }
}

pgspa::pgStoredProcAggThread::~pgStoredProcAggThread()
{
//  stop();
  delete PGDB;
  QSqlDatabase::removeDatabase(QString("PGConn_%1").arg(p_storage->config()->id));

  deleteLater();
}

void pgspa::pgStoredProcAggThread::stop()
{
  p_started = false;
  while(!p_finished) QCoreApplication::instance()->processEvents();
}

void pgspa::pgStoredProcAggThread::run()
{
  auto Var2Str = [](QVariant value) -> QString {

      switch (value.type()) {
        case QMetaType::Int:

          return value.isValid() ? QString::number(value.toDouble()) : "-1.0";
          break;

        case QMetaType::Double:

          return value.isValid() ? QString::number(value.toDouble()) : "-3.0";
          break;

        default:
          return "-1.0";

      }

  };

  QTime elapsed_time = QTime::currentTime();

  elapsed_time.start();

  p_started = true;
  p_finished = false;

  bool need_to_finish = false;

  while(!need_to_finish) {

    msleep(1); // чтоб не грузило систему

    if(elapsed_time.elapsed() < m_params.interval )
      continue;

    elapsed_time.restart();

    QString signals_values = "";
    QString signals_reserve_values = "";

    for(SvSignal* signal: *p_signals) {

      if(!p_started) // чтоб не перебирать все сигналы, если пришел stop
        break;

#ifdef TEST_VALUES
        qsrand(QDateTime::currentMSecsSinceEpoch());
        if(signal->config()->type == 0)
            signal->setValue(qreal((qrand()%2)));
        else
            signal->setValue(qreal((qrand() & 0xFF) / 255.0));
//        qDebug() << signal->value();
#endif

//      if((signal->config()->timeout >  0 && signal->isAlive()) ||
//         (signal->config()->timeout == 0 && signal->isDeviceAlive()))
      {
        signals_values.append(QString("%1;%2|").arg(signal->id()).arg(Var2Str(signal->value())));
      }
      /* алгоритм при потере сигнала:
       * 1) если для сигнала назначен резервный сигнал, то сигналу присваивается значение резервного сигнала
       * 2) если резервного сигнала нет, то присваиваем сигналу значение timeout_value,
       * 3) в первый раз зписываем в базу через функцию set_values. таким образом фиксируем в базе время потери связи
       * 4) впоследующем, если сигнал уже имеет значение timeout_value, то пропускаем его фиксацию в базе, для экономии ресурсов
       */
//      else {

//        if(signal->config()->timeout_signal_id > 0)
//        {
//          if(signals_reserv.contains(signal->config()->type))
//            signals_reserv[signal->config()->type] += QString("%1;%2|").arg(signal->id()).arg(signal->config()->timeout_signal_id);

//          else
//            signals_reserv.insert(signal->config()->type, QString("%1;%2|").arg(signal->id()).arg(signal->config()->timeout_signal_id));
//        }


//        else
//        {
//          /* если сигналу уже назначено значение timeout_value, то не добавляем сигнал к скрипту */
////          if(signal->previousValue() != signal->value())
//          {
//            if(signals_values.contains(signal->config()->type))
//              signals_values[signal->config()->type] += QString("%1;%2|").arg(signal->id()).arg(Var2Str(signal->value())); // signal->config()->timeout_value);

//            else
//              signals_values.insert(signal->config()->type, QString("%1;%2|").arg(signal->id()).arg(Var2Str(signal->value()))); // signal->config()->timeout_value));
//          }
//        }
//      }
    }

    /** здесь проверяем флаг p_started. если p_started = false, то есть пришел внешний сигнал на завершение потока,
     * то проходим по всем сигналам, и присваиваем им timeout_value.
     * присваиваем _need_to_finish = true. после прохода по всем сигналам, будет произведена запись в БД
     * на следующем прходе, главный цикл завершится, т.к. _need_to_finish уже true
     * такая схема применена для гарантированной записи в БД значений timeout_value при завершении работы сервера */
    if(!p_started)
    {
      for(SvSignal* signal: *p_signals)
        signals_values.append(QString("%1;%2|").arg(signal->id()).arg(Var2Str(QVariant())));

      need_to_finish = true;
    }


    try {

        if(!signals_values.isEmpty()) {

          signals_values.chop(1);

          QSqlError serr = PGDB->execSQL(QString(PG_FUNC_SET_VALUES)
                                         .arg(m_params.func_set_values)
                                         .arg(signals_values));

          if(serr.type() != QSqlError::NoError)
            throw SvException(serr.text());

        }

        if(!signals_reserve_values.isEmpty()) {

          signals_reserve_values.chop(1);
          QSqlError serr = PGDB->execSQL(QString(PG_FUNC_SET_RESERVE_VALUES).arg(signals_reserve_values));

          if(serr.type() != QSqlError::NoError)
            throw SvException(serr.text());

        }
      }


    catch(SvException& e) {

      emit error(e.error);

      /// если произошла потеря связи с серверм БД, то завершаем поток
      need_to_finish = PGDB->connected();
      p_started = PGDB->connected();      // это не работает. встает на db.exec и стоит пока соединение не появится снова

      /// если нет, то продолжаем работать

    }
  }

  p_finished = true;

}



/** ********** EXPORT ************ **/
as::SvAbstractStorage* create()
{
  return new pgspa::pgStoredProcAggStorage();
}

