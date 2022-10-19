#include "sv_tcp_server.h"

SvTcpServer::SvTcpServer()
{
}


bool SvTcpServer::configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer)
// Получение параметров интерфейса из конфигурационного файла устрйства.
{
  try {

    p_config = config;
    p_io_buffer = iobuffer;

    m_params = tcp::Params::fromJsonString(p_config->interface.params);

    return true;

  } catch (SvException& e) {

    p_last_error = e.error;
    return false;
  }
}


bool SvTcpServer::start()
// Создание и инициализация объектов; создание подключений сигналов к слотам,
// которые необходимы для работы tcp-сервера.
{
    // Создаём объект TCP-сервера:
    m_tcpServer = new QTcpServer(this);

    // Командуем ТСP-серверу начать прослушивать входящие соединения от клиента с
    // адресом и портом, указанными в конфигурационном файле для данного интерфейса:
    if (m_tcpServer->listen(m_params.listen_address, m_params.port) == false)
    {
        emit message(QString("TCP-сервер: Не удалось запустить TCP-сервер на прослушивание"), lldbg, mtfal);
        qDebug() << "TCP-сервер: Не удалось запустить TCP-сервер на прослушивание";
        return (true);
    }

    emit message(QString("TCP-cервер: TCP-cервер запущен на прослушивание"), lldbg, mtscc);
    qDebug() << "TCP-cервер: TCP-cервер запущен на прослушивание";

    // В слуае доступности нового соединия -> вызываем слот "newConnection":
    connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()), Qt::UniqueConnection);
    return (false);
}


void SvTcpServer::socketError(QAbstractSocket::SocketError err)
// Отображение в утилите "logview" ошибки сокета.
{
    emit message(QString ("TCP-сервер: ") + tcp::SocketErrors.value(err, QString("Неизвестная ошибка")), lldbg, mterr);
    qDebug() << QString ("TCP-сервер: ") + tcp::SocketErrors.value(err, QString("Неизвестная ошибка"));
}


void SvTcpServer::stateChanged(QAbstractSocket::SocketState state)
// Отображение в утилите "logview" изменившегося состояния подключения.
{
    emit message(QString ("TCP-сервер: ") + tcp::SocketStates.value(state, QString("Неизвестное состояние")), sv::log::llDebug, sv::log::mtConnection);
    qDebug() << QString ("TCP-сервер: ") + tcp::SocketStates.value(state, QString("Неизвестное состояние"));
}


void SvTcpServer::read()
// Получение данных от сокета клиентского подключения.
{
  qDebug() << "TCP-сервер: отладка затыкания - r1";
  m_gap_timer->stop();

  qDebug() << "TCP-сервер: отладка затыкания - r2";
  // Если нам надо читать данные от интерфейса в буфер, а протокольная часть ещё не прочла
  // прошлое содержание буфера, то стираем прошлое содержание.
  if(p_io_buffer->input->isReady())
  {
     qDebug() << "TCP-сервер: отладка затыкания - r3";
    p_io_buffer->input->reset();
  }
  qDebug() << "TCP-сервер: отладка затыкания - r4";
  p_io_buffer->input->mutex.lock();

  if(p_io_buffer->input->offset + m_clientConnection->bytesAvailable() > p_config->bufsize)
  {
      qDebug() << "TCP-сервер: отладка затыкания - r5";
      p_io_buffer->input->reset();
  }
//  qint64 readed = p_io_buffer->input->read(m_client);
  qint64 readed = m_clientConnection->read(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);
    qDebug() << "TCP-сервер: отладка затыкания - r6" << "Прочитано символов: "<< readed;

  if(p_io_buffer->input->offset == 0)
  { // Фиксируем момент НАЧАЛА чтения:
    p_io_buffer->input->set_time = QDateTime::currentMSecsSinceEpoch();
  }

  emit_message(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed), sv::log::llDebug, sv::log::mtReceive);

  p_io_buffer->input->offset += readed;

  p_io_buffer->input->mutex.unlock();

  // Прочтя поступившие от клиента данные, мы не сразу испускаем сигнал "dataReaded",
  // который говорит протокольной части о том что данные от интерфейса получены и
  // находятся в буфере.Это связано с тем, что, возможно, получен ещё не весь пакет прикладного
  // уровня (у нас это - уровень устройств и имитаторов). Поэтому мы
  // запускаем на небольшое время (по умолчанию 10 мс) таймер. Если до того, как таймер
  // сработает от интерфейса придут новые данные - мы дополним ими содержимое
  // буфера и перезапустим таймер. По срабатыванию таймера вызывается функция "newData",
  // которая испускает сигнал "dataReaded".
  m_gap_timer->start(m_params.grain_gap);
}


void SvTcpServer::newConnection()
// Функция вызывается, когда серверу доступно новое соединение с клиентом.
{
    if (m_clientConnection != nullptr)
    { // Если сокет клиентского подключения - создан:

        if (m_clientConnection->state() == QTcpSocket::ConnectedState)
        {
        // Типовой протокол А.2.2 по КСОН: КСОН проверяет, что нет соединения с таким же адресом и портом,
        // как у клиента, запрашивающего соединение. Если есть -> ОТКАЗ от установления соединения.

        // Реализуем это так: так как сервер слушает только указанные в конфигурационном файле адрес и порт,
        // то если соединение с клиентом уже есть -> новое - сбрасываем:
        return;
        }
    } // if (m_clientConnection...


    // Получаем TCP-сокет нового входящего подключения:
    m_clientConnection = m_tcpServer->nextPendingConnection();

    // По отключению клиента -> отображаем информацию об этом в утилите "logview" и уничтожаем его TCP-сокет:
    connect(m_clientConnection, &QTcpSocket::disconnected, this, &SvTcpServer::disconected, Qt::UniqueConnection);

    // Когда сокет клиента сообщает, что получил от него данные ->
    // вызываем функцию "SvTcp::read", которая прочтёт их и поместит
    // в буфер:
    connect(m_clientConnection, &QTcpSocket::readyRead, this, &SvTcpServer::read, Qt::UniqueConnection);

    // Когда протокольная часть сообщает, что поместила в буфер данные
    // для передачи по интерфейсу -> вызываем функцию "SvTcpServer::write",
    // которая запишет данные из буфера в сокет:
    connect(p_io_buffer, &modus::IOBuffer::readyWrite, this, &SvTcpServer::write, Qt::UniqueConnection);

    // Если произошла ошибка сокета -> отображаем информацию об этом в утилите "logview":
    connect(m_clientConnection, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &SvTcpServer::socketError, Qt::UniqueConnection);

    // Если изменилось состояние подключения -> отображаем информацию об этом в утилите "logview":
    connect(m_clientConnection, &QTcpSocket::stateChanged, this, &SvTcpServer::stateChanged, Qt::UniqueConnection);

    // Устанавливаем параметры таймера, по срабатыванию которого
    // испускаем сигнал "dataReaded". Более подробное описание см. в функции "SvTcpServer::read".
    m_gap_timer = new QTimer;
    m_gap_timer->setTimerType(Qt::PreciseTimer);
    m_gap_timer->setInterval(m_params.grain_gap);
    m_gap_timer->setSingleShot(true);
    connect(m_gap_timer, &QTimer::timeout, this, &SvTcpServer::newData, Qt::UniqueConnection);
}

void SvTcpServer::disconected()
// По отключению клиента -> отображаем информацию об этом в утилите "logview" и уничтожаем его TCP-сокет:
{
    // Отображаем информацию об отключении в утилите "logview":
    emit message(QString("TCP-сервер: Клиент отключился"), lldbg, mtfal);
    qDebug() << QString("TCP-сервер: Клиент отключился");

    // Уничтожаем сокет клиентского подключения:
    m_clientConnection ->deleteLater();

    // Уничтожаем таймер, используемый в фукции "SvTcpServer::read":
    m_gap_timer ->deleteLater();
}

void SvTcpServer::newData()
// После выполнения чтения из сокета клиента испускаем сигнал "dataReaded" с некоторой задержкой.
// Более подробное описание см. в функции "SvTcpServer::read"
{
  QMutexLocker(&p_io_buffer->input->mutex);

  qDebug() << "TCP-сервер: отладка затыкания - n1";
  QByteArray received = QByteArray((const char*)&p_io_buffer->input->data[0], p_io_buffer ->input ->offset); // Отладка

  emit_message((received), sv::log::llDebug, sv::log::mtReceive);

  qDebug() << "TCP-сервер: Принял: ";
  qDebug() << "TCP-сервер: Размер: " << received.length();
  qDebug() << "TCP-сервер: Содержание: " << received.toHex();

  p_io_buffer->input->setReady(true);
  emit p_io_buffer->dataReaded(p_io_buffer->input);
  qDebug() << "TCP-сервер: отладка затыкания - n2";
}


void SvTcpServer::write(modus::BUFF* buffer)
// Запись данных в сокет клиентского подключения:
{
  if(!buffer->isReady())
  {
    // Если данных для передачи нет, то нечего и передавать (выходим из функции):
      return;
  }
  if (m_clientConnection == nullptr)
  {
      // Если сокет сокет клиентского подключения не создан,
      // то некому и передавать (выходим из функции):
      return;
  }
  if (m_clientConnection -> state() != QAbstractSocket::ConnectedState)
  {
      // Если соединения с клиентом не установлено, то некому и передавать (выходим из функции):
      return;
  }

  buffer->mutex.lock();
  bool written = m_clientConnection->write((const char*)&buffer->data[0], buffer->offset) > 0;
  m_clientConnection->flush();

  if(written)
  {
    QByteArray sended = QByteArray((const char*)&buffer->data[0], buffer->offset);

    emit_message(sended, sv::log::llDebug, sv::log::mtSend);

    qDebug() << "TCP-сервер: Передал: ";
    qDebug() << "TCP-сервер: Размер: " << sended.length();
    qDebug() << "TCP-сервер: Содержание: " << sended.toHex();
  }

  buffer->reset();

  buffer->mutex.unlock();
}


void SvTcpServer::emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type)
{
  QString msg = "";

  //! The append() function is typically very fast
  switch (m_params.fmt) {
    case modus::HEX:
      msg.append(bytes.toHex());
      break;

    case modus::ASCII:
      msg.append(bytes);
      break;

    case modus::DATALEN:
      msg = QString("%1 байт %2").arg(bytes.length()).arg(type == sv::log::mtSend ? "отправлено" : type == sv::log::mtReceive ? "принято" : "");
      break;

    default:
      return;
      break;
  }

  emit message(msg, level, type);

}

/** ********** EXPORT ************ **/
modus::SvAbstractInterface* create()
{
  modus::SvAbstractInterface* device = new SvTcpServer();
  return device;
}

const char* getDefaultParams()
{
  return "{ \"host\": \"172.16.4.11\", \"recv_port\": 6001, \"send_port\": 5001 }";
}

const char* getName()
{
  return "Драйвер для работы TCP-сервера";
}

const char* getDescription()
{
  return "Драйвер для работы TCP-сервера";
}
