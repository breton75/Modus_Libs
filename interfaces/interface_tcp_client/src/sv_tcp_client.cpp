#include "sv_tcp_client.h"

SvTcpClient::SvTcpClient():
  m_client(nullptr),
  m_gap_timer(nullptr),
  m_connectionCheckTimer(nullptr)
{
}

SvTcpClient::~SvTcpClient()
{
  stop();

  m_gap_timer->stop();
  delete m_gap_timer;

  deleteLater();
}


bool SvTcpClient::configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer)
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


bool SvTcpClient::start()
// Создание и инициализация объектов; создание подключений сигналов к слотам,
// которые необходимы для работы tcp-клиента.
{
    // Создаём объект TCP-клиента:
    m_client = new QTcpSocket;

    // Сбрасываем флаг, говорящий о том, что выполняется команда разрыва TCP-соединения
      // с сервером:
//    breakConnectionCommand = false;

    // После подключения -> вызываем слот "connected":
    connect(m_client,     &QTcpSocket::connected,       this, &SvTcpClient::connected);

    // По отключении -> отображаем информацию об этом в утилите "logview":
    connect(m_client,     &QTcpSocket::disconnected,    this, &SvTcpClient::disconnected);

    // Когда сокет сообщает, что получил от сервера данные ->
    // вызываем функцию "SvTcpClient::read", которая прочтёт их и поместит
    // в буфер:
    connect(m_client,     &QTcpSocket::readyRead,       this, &SvTcpClient::read );

    // Когда протокольная часть сообщает, что поместила в буфер данные
    // для передачи по интерфейсу -> вызываем функцию "SvTcpClient::write",
    // которая запишет данные из буфера в сокет:
    connect(p_io_buffer,  &modus::IOBuffer::readyWrite, this, &SvTcpClient::write);

    // Когда от протокольной части поступает сигнал "say" -> вызывем функцию "say_WorkingOut",
    // чтобы выяснить, какую команду он "требует" и выполнить её:
    connect(p_io_buffer, &modus::IOBuffer::say, this, &SvTcpClient::say_WorkingOut);

    p_is_active = true;

    // Даём TCP-клиенту команду на подключение к серверу c
    // адресом и портом, указанными в конфигурационном файле для данного интерфейса:
    checkConnection();

    // На момент запуска TCP-клиента, TCP-сервер может быть не запущен, поэтому
    // мы по таймеру будем c интервалом, указанным в параметре интерфейса, вызывать
    // функцию "checkConnection", которая
    // будет проверять установлено ли TCP-соединение и, если оно не установлено, пытаться
    // его установить:
    m_connectionCheckTimer = new QTimer;
    connect(m_connectionCheckTimer, &QTimer::timeout, this, &SvTcpClient::checkConnection);
    m_connectionCheckTimer->start(m_params.reconnect_period);

    // Устанавливаем параметры таймера, по срабатыванию которого, вызываем функцию "newData", которая
    // устанавливает флаг "is_ready" и испускает сигнал "dataReaded".
    // Более подробное описание см. в функции "SvTcpServer::read".
    m_gap_timer = new QTimer;
    m_gap_timer->setTimerType(Qt::PreciseTimer);
    m_gap_timer->setInterval(m_params.grain_gap);
    m_gap_timer->setSingleShot(true);
    connect(m_gap_timer, &QTimer::timeout, this, &SvTcpClient::newData);

    return true;
}


void SvTcpClient::connected(void)
// По подключению к серверу -> отображаем информацию об этом
// в утилите "logview:
{
    emit message(QString("TCP-клиент: Успешно подключились к TCP-серверу"), lldbg, mtscc);
    qDebug() << QString("TCP-клиент: Успешно подключились к TCP-серверу");
}


void SvTcpClient::disconnected (void)
// По отключению от сервера -> отображаем информацию об этом
// в утилите "logview":
{
    // Мы оказались в состоянии отлючения от сервера -> сбрасываем флаг "breakConnectionCommand",
    // чтобы возобновить попытки установить соединение (этого требует проткол обмена с КСОН):
    p_is_active = false;

    emit message(QString("TCP-клиент: Отключились от TCP-сервера"), lldbg, mtscc);
    qDebug() << QString("TCP-клиент: Отключились от TCP-сервера");
}


void SvTcpClient::socketError(QAbstractSocket::SocketError err)
// Отображение в утилите "logview" ошибки сокета.
{
  emit message(QString ("TCP-клиент: ") + tcp::SocketErrors.value(err, QString("Неизвестная ошибка")), lldbg, mterr);
  qDebug() << QString ("TCP-клиент: ") + tcp::SocketErrors.value(err, QString("Неизвестная ошибка"));
}


void SvTcpClient::stateChanged(QAbstractSocket::SocketState state)
// Отображение в утилите "logview" изменившегося состояния подключения.
{
    emit message(QString ("TCP-клиент: ") + tcp::SocketStates.value(state, QString("Неизвестное состояние")), sv::log::llDebug, sv::log::mtConnection);
    qDebug() << QString ("TCP-клиент: ") + tcp::SocketStates.value(state, QString("Неизвестное состояние"));
}

void SvTcpClient::checkConnection(void)
// На момент запуска TCP-клиента, TCP-сервер может быть не запущен, поэтому
// мы по таймеру будем c интервалом, указанным в параметре интерфейса, вызывать
// функцию "checkConnection", которая
// будет проверять установлено ли TCP-соединение и, если оно не установлено, пытаться
// его установить:
{
    // Если выполняется команда разрыва TCP-соединения с сервером, то до того, как
    // соединение будет разорвано, пытаться его установить - не нужно:
    if (!p_is_active)
        return;

     qDebug() << "TCP-клиент: выполняется проверка соединения с сервером";

    // Проверим состояние соединения:
    if (m_client && m_client->state() != QAbstractSocket::ConnectedState)
    { // Если соединение с сервером не установлено, то пытаемся его установить:

        qDebug() << "TCP-клиент: Даём команду на подслоединение к серверу";
        m_client->connectToHost(m_params.server_address, m_params.port);
        return;
    }
}

void SvTcpClient::read()
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


void SvTcpClient::newData(void)
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


void SvTcpClient::write(modus::BUFF* buffer)
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


void SvTcpClient::say_WorkingOut(QByteArray command)
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
      connect(m_connectionCheckTimer, &QTimer::timeout, this, &SvTcpClient::checkConnection);
      m_connectionCheckTimer->start(m_params.reconnect_period);

    }
}

void SvTcpClient::stop()
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

void SvTcpClient::emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type)
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
  modus::SvAbstractInterface* device = new SvTcpClient();
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
