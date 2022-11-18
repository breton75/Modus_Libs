#include "tcp_client_multi.h"

#define CURRENT_CAN_ENTRY "current_can_entry"

SvTcpClientMulti::SvTcpClientMulti():
  m_client(nullptr),
  m_gap_timer(nullptr),
  m_connectionCheckTimer(nullptr)
{
}

SvTcpClientMulti::~SvTcpClientMulti()
{
  stop();

  m_gap_timer->stop();
  delete m_gap_timer;

  deleteLater();
}


bool SvTcpClientMulti::configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer)
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


bool SvTcpClientMulti::start()
// Создание и инициализация объектов; создание подключений сигналов к слотам,
// которые необходимы для работы tcp-клиента.
{
    // Создаём объект TCP-клиента:
    m_client = new QTcpSocket;

    m_client->setProperty(CURRENT_CAN_ENTRY, QVariant());

    // Сбрасываем флаг, говорящий о том, что выполняется команда разрыва TCP-соединения
      // с сервером:
//    breakConnectionCommand = false;

    // После подключения -> вызываем слот "connected":
    connect(m_client,     &QTcpSocket::connected,       this, &SvTcpClientMulti::connected);

    // По отключении -> отображаем информацию об этом в утилите "logview":
    connect(m_client,     &QTcpSocket::disconnected,    this, &SvTcpClientMulti::disconnected);

    connect(m_client,     &QTcpSocket::stateChanged,    this, &SvTcpClientMulti::stateChanged);
    connect(m_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
//    connect(m_client,     &QTcpSocket::error,           this, &SvTcpClientMulti::socketError);

    // Когда сокет сообщает, что получил от сервера данные ->
    // вызываем функцию "SvTcpClient::read", которая прочтёт их и поместит
    // в буфер:
    connect(m_client,     &QTcpSocket::readyRead,       this, &SvTcpClientMulti::read );

    // Когда протокольная часть сообщает, что поместила в буфер данные
    // для передачи по интерфейсу -> вызываем функцию "SvTcpClient::write",
    // которая запишет данные из буфера в сокет:
    connect(p_io_buffer,  &modus::IOBuffer::readyWrite, this, &SvTcpClientMulti::write);

    // Когда от протокольной части поступает сигнал "say" -> вызывем функцию "say_WorkingOut",
    // чтобы выяснить, какую команду он "требует" и выполнить её:
    connect(p_io_buffer, &modus::IOBuffer::say, this, &SvTcpClientMulti::say_WorkingOut);

    p_is_active = true;

    m_connectionCheckTimer = new QTimer;
    connect(m_connectionCheckTimer, &QTimer::timeout, this, &SvTcpClientMulti::checkConnection);
    m_connectionCheckTimer->setSingleShot(true);
//    m_connectionCheckTimer->start(m_params.reconnect_period);

    // Даём TCP-клиенту команду на подключение к серверу c
    // адресом и портом, указанными в конфигурационном файле для данного интерфейса:
    checkConnection();

    // На момент запуска TCP-клиента, TCP-сервер может быть не запущен, поэтому
    // мы по таймеру будем c интервалом, указанным в параметре интерфейса, вызывать
    // функцию "checkConnection", которая
    // будет проверять установлено ли TCP-соединение и, если оно не установлено, пытаться
    // его установить:

    // Устанавливаем параметры таймера, по срабатыванию которого, вызываем функцию "newData", которая
    // устанавливает флаг "is_ready" и испускает сигнал "dataReaded".
    // Более подробное описание см. в функции "SvTcpServer::read".
    m_gap_timer = new QTimer;
    m_gap_timer->setTimerType(Qt::PreciseTimer);
    m_gap_timer->setInterval(m_params.grain_gap);
    m_gap_timer->setSingleShot(true);
    connect(m_gap_timer, &QTimer::timeout, this, &SvTcpClientMulti::newData);

    return true;
}


void SvTcpClientMulti::connected(void)
// По подключению к серверу -> отображаем информацию об этом
// в утилите "logview:
{
    emit message(QString("TCP-клиент: Успешно подключились к TCP-серверу"), lldbg, mtscc);
    qDebug() << QString("TCP-клиент: Успешно подключились к TCP-серверу");
}


void SvTcpClientMulti::disconnected (void)
// По отключению от сервера -> отображаем информацию об этом
// в утилите "logview":
{
    // Мы оказались в состоянии отлючения от сервера -> сбрасываем флаг "breakConnectionCommand",
    // чтобы возобновить попытки установить соединение (этого требует проткол обмена с КСОН):
//    p_is_active = false;

    emit message(QString("TCP-клиент: Отключились от TCP-сервера"), lldbg, mtscc);
    qDebug() << QString("TCP-клиент: Отключились от TCP-сервера");
}


void SvTcpClientMulti::socketError(QAbstractSocket::SocketError err)
// Отображение в утилите "logview" ошибки сокета.
{
  emit message(QString ("TCP-клиент: ") + tcp::SocketErrors.value(err, QString("Неизвестная ошибка")), lldbg, mterr);
  qDebug() << QString ("TCP-клиент: ") + tcp::SocketErrors.value(err, QString("Неизвестная ошибка"));
}


void SvTcpClientMulti::stateChanged(QAbstractSocket::SocketState state)
// Отображение в утилите "logview" изменившегося состояния подключения.
{
    emit message(QString ("TCP-клиент: ") + tcp::SocketStates.value(state, QString("Неизвестное состояние")), sv::log::llDebug, sv::log::mtConnection);
    qDebug() << QString ("TCP-клиент: ") + tcp::SocketStates.value(state, QString("Неизвестное состояние"));
}

void SvTcpClientMulti::checkConnection(void)
// На момент запуска TCP-клиента, TCP-сервер может быть не запущен, поэтому
// мы по таймеру будем c интервалом, указанным в параметре интерфейса, вызывать
// функцию "checkConnection", которая
// будет проверять установлено ли TCP-соединение и, если оно не установлено, пытаться
// его установить:
{

  char states[m_params.connections.count()];
  memset(&states[0], 0, m_params.connections.count());

  // Если выполняется команда разрыва TCP-соединения с сервером, то до того, как
  // соединение будет разорвано, пытаться его установить - не нужно:
  if (p_is_active && m_client) {

     qDebug() << "TCP-клиент: выполняется проверка соединения с сервером"; // << m_params.connections.count();

     // Если соединение с сервером не установлено, то пытаемся его установить:
     // пытаемся по очереди подключиться к серверам, заданным в списке connections
     for(int i = 0; i < m_params.connections.count(); i++) {
qDebug() << 0 << "i=" << i << m_client->state();
       if(m_client->state() == QAbstractSocket::ConnectedState) {

         if(m_params.connections.at(i).socketDescriptor == m_client->socketDescriptor()) {

           states[i] = 2;
           continue;
         }
         else {

           // проверяем доступность очередного сервера
           m_params.connections[i].socket = new QTcpSocket;
           m_params.connections[i].socket ->connectToHost(m_params.connections.at(i).host, m_params.connections.at(i).port);
           states[i] = m_params.connections[i].socket->waitForConnected(1000) ? 1 : 0;

           m_params.connections[i].socket->disconnectFromHost();
           delete m_params.connections[i].socket;
           m_params.connections[i].socket = nullptr;
           m_params.connections[i].socketDescriptor = -1;

qDebug() << 2 << "i=" << i << m_params.connections.at(i).host.toString() << m_params.connections.at(i).port;
         }
       }
       else {

//         if(m_params.connections[i].socket) {
//qDebug() << "magic" << i << m_params.connections[i].socket;
//           try {
//             delete m_params.connections[i].socket;
//           }
//           catch(...){
//             qDebug() << "wrong tcp socket pointer";
//           }

           m_params.connections[i].socketDescriptor = -1; //nullptr;


//         }
qDebug() << 1 << "i=" << i << m_params.connections.at(i).host.toString() << m_params.connections.at(i).port;
         m_client->connectToHost(m_params.connections.at(i).host, m_params.connections.at(i).port);

         if(m_client->waitForConnected(1000)) {

           int fd = m_client->socketDescriptor();
           int enableKeepAlive = 1;
           setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));

           int maxIdle = 2; /* seconds */
           setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

           int count = 1;  // send up to 3 keepalive packets out, then disconnect if no response
           setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

           int interval = 1;   // send a keepalive packet out every 2 seconds (after the 5 second idle period)
           setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));

           m_params.connections[i].socketDescriptor = m_client->socketDescriptor();
           states[i]  = 2;

         }
         else {

           m_params.connections[i].socketDescriptor = -1;
           // для серверов, к которым не удалось подключиться states[i] останется
           // равным 0 - признак отстутствия подключения

         }
       }
     }
  }

  p_io_buffer->state->mutex.lock();
  p_io_buffer->state->setData(&states[0], m_params.connections.count());
  p_io_buffer->state->mutex.unlock();

  qDebug() << states[0] + 48 << states[1] + 48 << states[2] + 48 << states[3] + 48;
  emit p_io_buffer->notice(p_io_buffer->state);

  m_connectionCheckTimer->start(m_params.reconnect_period);
}

void SvTcpClientMulti::read()
// Получение данных из сокета.
{
    m_gap_timer->stop();

    p_io_buffer->input->mutex.lock();

   // Если нам надо читать данные от сокета в буфер, а протокольная часть ещё не прочла
   // прошлое содержание буфера, то сбрасываем флаг "is_ready", при этом данные,
   // получаемые от сокета будут добавляться к тем данным, которые уже имеются в буфере:
   if(p_io_buffer->input->isReady())
       p_io_buffer->input->is_ready = false;

   // Если места в буфере на новые данные от сокета нет, то очищаем его содержимое и
   // сбрасываем флаг "is_ready":
   if(p_io_buffer->input->offset + m_client->bytesAvailable() > p_config->bufsize)
        p_io_buffer->input->reset();


   qint64 readed = m_client->read(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);

   if(p_io_buffer->input->offset == 0)
   { // Фиксируем момент НАЧАЛА чтения:
        p_io_buffer->input->set_time = QDateTime::currentMSecsSinceEpoch();
   }

  emit_message(QByteArray((const char*)&p_io_buffer->input->data[p_io_buffer->input->offset], readed), sv::log::llDebug, sv::log::mtReceive);

  p_io_buffer->input->offset += readed;

  p_io_buffer->input->mutex.unlock();

  // Прочтя поступившие от сервера данные, мы не сразу устанавливаем флаг "is_ready" и
  // испускаем сигнал "dataReaded",
  // которые говорит протокольной части о том что данные от интерфейса получены и
  // находятся в буфере.Это связано с тем, что, возможно, получен ещё не весь пакет прикладного
  // уровня (у нас это - уровень устройств и имитаторов). Поэтому мы
  // запускаем на небольшое время (по умолчанию 10 мс) таймер. Если до того, как таймер
  // сработает от интерфейса придут новые даннные - мы дополним ими содержимое
  // буфера и перезапустим таймер. По срабатыванию таймера вызывается функция "newData",
  // которая испускает сигнал "dataReaded".
  m_gap_timer->start(m_params.grain_gap);
}


void SvTcpClientMulti::newData(void)
// После выполнения чтения из сокета клиента устанавливаем флаг "is_ready" и
// испускаем сигнал "dataReaded" с некоторой задержкой.
// Более подробное описание см. в функции "SvTcpServer::read"
{
   QByteArray received = QByteArray((const char*)&p_io_buffer->input->data[0], p_io_buffer ->input ->offset); // Отладка

   //qDebug() << "TCP-клиент: Принял: ";
   //qDebug() << "TCP-клиент: Размер: " << received.length();
   //qDebug() << "TCP-клиент: Содержание: " << received.toHex();

   QMutexLocker(&p_io_buffer->input->mutex);
   p_io_buffer->input->setReady(true);
   emit p_io_buffer->dataReaded(p_io_buffer->input);
}


void SvTcpClientMulti::write(modus::BUFF* buffer)
// Запись данных в сокет:
{
  if(!buffer->isReady())
  {
    // Если данных для передачи нет, то нечего и передавать (выходим из функции):
    return;
  }

  if (m_client -> state() != QAbstractSocket::ConnectedState)
  {
      // Если соединения с сервером не установлено, то некому и передавать (выходим из функции):
      return;
  }

  buffer->mutex.lock();

  bool written = m_client->write((const char*)&buffer->data[0], buffer->offset) > 0;
  m_client->flush();

  if(written)
  {
    QByteArray sended = QByteArray((const char*)&buffer->data[0], buffer->offset);

    emit_message(sended, sv::log::llDebug, sv::log::mtSend);

    //qDebug() << "TCP-клиент: Передал: ";
    //qDebug() << "TCP-клиент: Размер: " << sended.length();
    //qDebug() << "TCP-клиент: Содержание: " << sended.toHex();
  }

  buffer->reset();

  buffer->mutex.unlock();
}


void SvTcpClientMulti::say_WorkingOut(QByteArray command)
// Выяснение, какую команду требуется выполнить, и её выполнение.
// Аргумент "command" - требуемая команда.
{
    if (QString(command).toLower() == QString("disconnect")) {

      this->stop();

    }
    else if(QString(command).toLower() == "connect") {

      p_is_active = true;

      checkConnection();

      // На момент запуска TCP-клиента, TCP-сервер может быть не запущен, поэтому
      // мы по таймеру будем c интервалом, указанным в параметре интерфейса, вызывать
      // функцию "checkConnection", которая
      // будет проверять установлено ли TCP-соединение и, если оно не установлено, пытаться
      // его установить:
      m_connectionCheckTimer = new QTimer;
      connect(m_connectionCheckTimer, &QTimer::timeout, this, &SvTcpClientMulti::checkConnection);
      m_connectionCheckTimer->start(m_params.reconnect_period);

    }
}

void SvTcpClientMulti::stop()
{
  p_is_active = false;

  // отключаем все сигналы таймера от всех слотов
  m_connectionCheckTimer->disconnect();
  m_connectionCheckTimer->stop();
  delete m_connectionCheckTimer;
  m_connectionCheckTimer = nullptr;

  // Даём команду на отключение от TCP-сервера:
  if(m_client && m_client->state() == QAbstractSocket::ConnectedState)
    m_client->disconnectFromHost();

}

void SvTcpClientMulti::emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type)
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
  modus::SvAbstractInterface* device = new SvTcpClientMulti();
  return device;
}

const char* getVersion()
{
  return LIB_VERSION;
}

const char* getParams()
{
  return tcp::Params::usage().toStdString().c_str();
}

const char* getInfo()
{
  return LIB_SHORT_INFO;
}

const char* getDescription()
{
  return LIB_DESCRIPTION;
}
