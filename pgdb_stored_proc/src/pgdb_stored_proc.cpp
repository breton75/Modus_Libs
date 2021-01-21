#include "pgdb_stored_proc.h"


pgsp::pgStoredProcStorage::pgStoredProcStorage()
{
  p_signals->clear();

  m_reconnect_timer = new QTimer();
  m_reconnect_timer->setInterval(1000);

}

pgsp::pgStoredProcStorage::~pgStoredProcStorage()
{
  delete PGDB;
  QSqlDatabase::removeDatabase(QString("PGConn_%1").arg(p_config->id));

  deleteLater();

}

bool pgsp::pgStoredProcStorage::init(modus::StorageConfig* config)
{
  p_config = config;

  try {

    /* парсим - проверяем, что парметры заданы верно */
    m_params = pgsp::Params::fromJson(p_config->params);

    return true;

  }

  catch(SvException& e) {

    p_last_error = e.error;

    return false;
  }
}

bool pgsp::pgStoredProcStorage::connect()
{
  try {

    if(PGDB) {
      delete PGDB;
      QSqlDatabase::removeDatabase(QString("PGConn_%1").arg(p_config->id));
    }

    PGDB = new SvPGDB();
    PGDB->setConnectionParams(m_params.db, m_params.host, m_params.port,
                              m_params.login, m_params.pass, m_params.role);

    QSqlError err = PGDB->connectToDB(QString("PGConn_%1").arg(p_config->id));

    if(err.type() != QSqlError::NoError)
      throw SvException(err.text());

    return true;

  }

  catch(SvException& e) {

    p_last_error = e.error;
    emit message(e.error, sv::log::llError, sv::log::mtError);

    return false;

  }
}

//void pgsp::pgStoredProcStorage::reconnect()
//{
//  if(connect()) {

//    stop_reconnect_timer();
//    start();

//    logreconnect();

//  }
//}

//void pgsp::pgStoredProcStorage::start_reconnect_timer()
//{
////  m_reconnect_timer->start();
////  connect(m_reconnect_timer, &QTimer::timeout, this, &pgsp::pgStoredProcStorage::reconnect);
//}

//void pgsp::pgStoredProcStorage::stop_reconnect_timer()
//{
////  m_reconnect_timer->stop();
////  disconnect(m_reconnect_timer, &QTimer::timeout, this, &pgsp::pgStoredProcStorage::reconnect);
//}

void pgsp::pgStoredProcStorage::stop()
{
  /// при ручном завершении, отключаем переподключение
//  disconnect(p_thread, &pgsp::pgStoredProcThread::finished, this, &pgsp::pgStoredProcStorage::start_reconnect_timer);
//  disconnect((pgsp::pgStoredProcThread*)p_thread, &pgsp::pgStoredProcThread::connected, this, &pgsp::pgStoredProcStorage::logreconnect);

//  stop_reconnect_timer();
//  delete m_reconnect_timer;

//  p_thread->stop();

}

void pgsp::pgStoredProcStorage::run()
{
  auto Var2Str = [](QVariant value) -> QString {

      if(value.isValid())
      {
        switch (value.type()) {
          case QVariant::Int:

            return QString::number(value.toInt());
            break;

          case QVariant::Double:

            return QString::number(value.toDouble());
            break;

          default:
            return "NULL";

        }
      }

      return "NULL";

  };

  QTime elapsed_time = QTime::currentTime();

  elapsed_time.start();

  p_is_active = true;

  bool need_to_finish    = false;
  bool need_to_reconnect = true;

  while(!need_to_finish) {

    if(need_to_reconnect && !connect())
      continue;

    need_to_reconnect = false;

    msleep(1); // чтоб не грузило систему

    if(quint64(elapsed_time.elapsed()) < p_config->interval )
      continue;

    elapsed_time.restart();

    QMap<QString, QString> signals_values;

    for(modus::SvSignal* signal: *p_signals) {

      if(!p_is_active) // чтоб не перебирать все сигналы, если пришел stop
        break;

#ifdef TEST_VALUES
        qsrand(QDateTime::currentMSecsSinceEpoch());
        if(signal->config()->type == 0)
            signal->setValue(qreal((qrand()%2)));
        else
            signal->setValue(qreal((qrand() & 0xFF) / 255.0));
//        qDebug() << signal->value();
#endif

      {
        if(signals_values.contains(signal->config()->type))
          signals_values[signal->config()->type] += QString("%1;%2|").arg(signal->id()).arg(Var2Str(signal->value()));

        else
          signals_values.insert(signal->config()->type, QString("%1;%2|").arg(signal->id()).arg(Var2Str(signal->value())));

      }
    }

    /** здесь проверяем флаг p_is_active. если p_is_active = false, то есть пришел внешний сигнал на завершение потока,
     * то проходим по всем сигналам, и присваиваем им timeout_value.
     * присваиваем _need_to_finish = true. после прохода по всем сигналам, будет произведена запись в БД
     * на следующем прходе, главный цикл завершится, т.к. _need_to_finish уже true
     * такая схема применена для гарантированной записи в БД значений timeout_value при завершении работы сервера */
    if(!p_is_active)
    {
      for(modus::SvSignal* signal: *p_signals) {

        if(signals_values.contains(signal->config()->type))
          signals_values[signal->config()->type] += QString("%1;%2|").arg(signal->id()).arg(Var2Str(QVariant()));

        else
          signals_values.insert(signal->config()->type, QString("%1;%2|").arg(signal->id()).arg(Var2Str(QVariant())));

      }

      need_to_finish = true;
    }


    try {

      foreach (QString type, signals_values.keys()) {

        if(!signals_values.value(type).isEmpty()) {

          signals_values[type].chop(1);

          QSqlError serr = PGDB->execSQL(QString(PROC_CALL)
                                         .arg(m_params.proc_name)
                                         .arg(type)
                                         .arg(signals_values.value(type)));

          if(serr.type() != QSqlError::NoError)
            throw SvException(serr.text());

        }
      }
    }

    catch(SvException& e) {

      emit message(e.error, sv::log::llError, sv::log::mtError);

      // если произошла потеря связи с серверм БД, то завершаем поток
      need_to_reconnect = !PGDB->connected();      // это не работает. встает на db.exec и стоит пока соединение не появится снова

      // если нет, то продолжаем работать

    }
  }
}



/** ********** EXPORT ************ **/
modus::SvAbstractStorage* create()
{
  return new pgsp::pgStoredProcStorage();
}

