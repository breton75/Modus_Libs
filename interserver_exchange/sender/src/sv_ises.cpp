#include "sv_ises.h"


ises::SvISES::SvISES(sv::SvAbstractLogger* logger):
  as::SvAbstractStorage(logger)
{
  p_signals.clear();

  m_reconnect_timer = new QTimer();
  m_reconnect_timer->setInterval(1000);

}

ises::SvISES::~SvISES()
{
  deleteLater();
}

bool ises::SvISES::configure(const as::StorageConfig& config)
{
  p_config = config;

  try {

    /* парсим - проверяем, что парметры заданы верно */
    m_params = ises::StorageParams::fromJson(p_config.params);

    p_is_configured = true;

    return true;

  }

  catch(SvException& e) {

    p_is_configured = false;
    p_last_error = e.error;

    return false;
  }
}

bool ises::SvISES::init()
{
  try {

    p_thread = new ises::SvISESThread(this);

    if(!p_thread->init())
      throw SvException(p_thread->lastError());

    return true;

  }

  catch(SvException& e) {

    p_last_error = e.error;

    delete p_thread;
    p_thread = nullptr;

    logerr(p_last_error);

    return false;

  }
}

void ises::SvISES::logerr(QString e)
{
  *p_logger << sv::log::TimeZZZ << sv::log::llError << e << '\n' << sv::log::endl;
}

void ises::SvISES::logreconnect()
{
  if(p_logger)
    *p_logger << sv::log::TimeZZZ << sv::log::llInfo << QString("Фух! Восстановлена связь с хранилищем %1 [%2:%3:%4]\n")
                    .arg(p_config.name).arg(m_params.db)
                    .arg(m_params.host).arg(m_params.port)
              << sv::log::endl;
}

void ises::SvISES::stop()
{
  if(!p_thread)
    return;

  /// при ручном завершении, отключаем переподключение
  disconnect(p_thread, &ises::SvISESThread::finished, this, &ises::SvISESThread::deleteLater);

  p_thread->stop();

}

void ises::SvISES::start()
{
  if(!p_thread)
    return;

//  connect(_thr, &ises::SvISESThread::finished, _thr, &ises::SvISESThread::deleteLater);
  connect(p_thread, &ises::SvISESThread::finished, this, &ises::SvISESThread::deleteLater);
//  connect(_thr, &ises::SvISESThread::finished, [=](){ delete _thr; _thr = nullptr; } );
  connect((ises::SvISESThread*)p_thread, &ises::SvISESThread::error, this, &ises::SvISES::logerr);

  p_thread->start();

}

void ises::SvISES::deleteThread()
{
  delete p_thread;
  p_thread = nullptr;
}

/** *************  Storage Thread  ******************* */

ises::SvISESThread::SvISESThread(as::SvAbstractStorage* storage):
  as::SvAbstractStorageThread(storage)
{

}

bool ises::SvISESThread::init()
{
  try {

    m_params = ises::StorageParams::fromJson(p_storage->config()->params);

    return true;

  }

  catch(SvException& e) {

    p_last_error = e.error;
    emit error(e.error);

    return false;

  }
}

ises::SvISESThread::~SvISESThread()
{
  deleteLater();
}

void ises::SvISESThread::stop()
{
  p_started = false;
  while(!p_finished) QCoreApplication::instance()->processEvents();
}

void ises::SvISESThread::run()
{
  QUdpSocket socket;
  QByteArray datagram = QByteArray();
  QByteArray varmap = QByteArray();
  QDataStream stream(&varmap, QIODevice::WriteOnly);
  stream.setVersion(QDataStream::Qt_5_5);

  QTime elapsed_time = QTime::currentTime();

  elapsed_time.start();

  p_started = true;
  p_finished = false;

  while(p_started) {

    msleep(1); // чтоб не грузило систему

    if(elapsed_time.elapsed() < m_params.send_interval )
      continue;

    elapsed_time.restart();

    QVariantMap signals_values;

    for(SvSignal* signal: *p_signals) {

      if(!p_started) // чтоб не перебирать все сигналы, если пришел stop
        break;

      // по-хорошему здесь надо делать проверкк на уникальность имени,
      // но такая проверка делается раньше и не должно быть не уникальных имен
      signals_values.insert(signal->config()->name, signal->value());

    }

    varmap.clear();
    stream << signals_values;

    quint16 varlen = varmap.length();

    datagram.clear();
    datagram.append(ise::DEF_SIGN)
            .append((const char*)&m_params.iseid,    sizeof(quint16))
            .append((const char*)&m_params.receiver, sizeof(quint16))
            .append((const char*)&varlen, sizeof(quint16))
            .append(varmap);

    quint16 crc = CRC::MODBUS_CRC16((const char*)(datagram.data()), sizeof(ise::Header) + varlen);

    datagram.append((const char*)&crc, sizeof(quint16));

    socket.writeDatagram(datagram, m_params.host, m_params.port);
    socket.flush();

  }

  p_finished = true;

}



/** ********** EXPORT ************ **/
as::SvAbstractStorage* create()
{
  return new ises::SvISES();
}

