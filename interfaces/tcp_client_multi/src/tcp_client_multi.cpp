#include "tcp_client_multi.h"

#define CURRENT_CAN_ENTRY "current_can_entry"

SvTcpClientMulti::SvTcpClientMulti():
  m_client_list(QList<QTcpSocket*>()),
  m_current_connection(nullptr),
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
  // Создаём сокеты TCP-клиента, в количестве заданном параметром connections
  for(int i = 0; i < m_params.connections.count(); i++) {

    m_params.connections[i].socket = new QTcpSocket;

    // созданные сокеты поключаем к слотам connected и disconnected, для отслеживания этих событий
    connect(m_params.connections[i].socket,     &QTcpSocket::connected,       this, &SvTcpClientMulti::connected);
    connect(m_params.connections[i].socket,     &QTcpSocket::disconnected,    this, &SvTcpClientMulti::disconnected);

    // слот readyRead переключаем в функции checkConnection на сокет, который имеет наибольший приоритет
//    connect(m_params.connections[i].socket,     &QTcpSocket::readyRead,       this, &SvTcpClientMulti::read );

//    connect(m_params.connections[i].socket,     &QTcpSocket::stateChanged,    this, &SvTcpClientMulti::stateChanged);
//    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
//    connect(client,     &QTcpSocket::error,           this, &SvTcpClientMulti::socketError);
  }

  // Когда протокольная часть сообщает, что поместила в буфер данные
  // для передачи по интерфейсу -> вызываем функцию "SvTcpClient::write",
  // которая запишет данные из буфера в сокет:
  connect(p_io_buffer,  &modus::IOBuffer::readyWrite, this, &SvTcpClientMulti::write);

  // Когда от протокольной части поступает сигнал "say" -> вызывем функцию "say_WorkingOut",
  // чтобы выяснить, какую команду он "требует" и выполнить её:
  connect(p_io_buffer, &modus::IOBuffer::say, this, &SvTcpClientMulti::say_WorkingOut);

  p_is_active = true;

  // На момент запуска TCP-клиента, TCP-сервер может быть не запущен, поэтому
  // мы по таймеру будем c интервалом, указанным в параметре интерфейса, вызывать
  // функцию "checkConnection", которая будет проверять установлено ли TCP-соединение и,
  // если оно не установлено, пытаться его установить:
  m_connectionCheckTimer = new QTimer;
  connect(m_connectionCheckTimer, &QTimer::timeout, this, &SvTcpClientMulti::checkConnection);
  m_connectionCheckTimer->setSingleShot(true);

  // Даём TCP-клиенту команду на подключение к серверу c
  // адресом и портом, указанными в конфигурационном файле для данного интерфейса:
  checkConnection();

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

// отслеживаем событие подключения для сокетов
void SvTcpClientMulti::connected(void)
{
  QTcpSocket* socket = (QTcpSocket*)(sender());

  if(socket) {

//    for(int i = 0; i < m_params.connections.count(); i++) {

//      if(m_params.connections.at(i).socket == socket) {

    // задаем параметры сокета, такие, чтобы при потере соединения, сокет дисконнектился
    // по умолчанию, сокет, даже при физическом разрыве соединения, показывает состояние ConnectedState
    // решение взято отсюда:
    // https://askdev.ru/q/sostoyanie-qtcpsocket-vsegda-podklyucheno-dazhe-otsoedinenie-ethernet-provoda-129854/
    int fd = socket->socketDescriptor();
    int enableKeepAlive = 1;
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));

    int maxIdle = 2; /* seconds */
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

    int count = 1;  // send up to 3 keepalive packets out, then disconnect if no response
    setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

    int interval = 1;   // send a keepalive packet out every 2 seconds (after the 5 second idle period)
    setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));

    QString m = QString("Успешно подключились к TCP-серверу %1:%2")
                .arg(socket->peerAddress().toString())
                .arg(socket->peerPort());

//                    .arg(m_params.connections.at(i).host.toString())
//                    .arg(m_params.connections.at(i).port);

    emit message(m, lldbg, mtscc);
    qDebug() << m;

//      }
//    }
  }
}

void SvTcpClientMulti::disconnected (void)
{
  QTcpSocket* socket = (QTcpSocket*)(sender());

  if(socket) {

    // если сокет, который дисконнектнулся, относится к текущему подключению, то
    // отключаем этот сокет от слота read и обнуляем текущее подключение (текущего подключения нет)
    if(m_current_connection && (m_current_connection->socket == socket)) {

      disconnect(m_current_connection->socket,     &QTcpSocket::readyRead,       this, &SvTcpClientMulti::read );
      m_current_connection = nullptr;
    }

    QString m = QString("Отключились от TCP-сервера %1:%2")
                .arg(socket->peerAddress().toString())
                .arg(socket->peerPort());

    emit message(m, lldbg, mtscc);
    qDebug() << m;

  }
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
  // массив для хранения текущих состояний подключений
  // после прохода по всем сокетам, заполняется состояниями и отправляется в протокольный модуль
  char states[m_params.connections.count()];
  memset(&states[0], 0, m_params.connections.count());

  // Если выполняется команда разрыва TCP-соединения с сервером, то до того, как
  // соединение будет разорвано, пытаться его установить - не нужно:
  if (p_is_active) {

    qDebug() << "TCP-клиент: выполняется проверка соединения с сервером";

    // проходим по очереди по всем подключениям, заданным в списке connections
    // анализируем и назначаем текущее подключение
    for(int i = 0; i < m_params.connections.count(); i++) {

      tcp::CanEntryConnection*  connection  = &m_params.connections[i];
      QTcpSocket*               client      = connection->socket;

      switch (client->state()) {

        // если для данного сокета не было установлено соединение
        case QAbstractSocket::UnconnectedState:
        {
          // отдельно пррверяем вариант, когда соединение было (и оно было текущим), но произошел разрыв.
          // тогда отключаем данный сокет от слота read и обнуляем текущее соединение (текущего соединения нет)
          if(m_current_connection && (m_current_connection->can_entry == connection->can_entry)) {

            // отключаемся от слота read
            disconnect(m_current_connection->socket,     &QTcpSocket::readyRead,       this, &SvTcpClientMulti::read );
            m_current_connection = nullptr;
          }

          // даем команду на подключение
          // данную команду выполняем асинхронно. здесь не ждем подключения, чтобы не блокировать поток
          // если ждать подключение здесь, то интервал ожидания (для 4 подключений) превышает
          // timeout сигналов и данные на экране мигают (становятся серыми, как будто нет данных, потом восстанавливаются)
          // если соединение будет установлено, то для данного сокета будет вызван слот connected
          // и при следующем вызове checkConnection, будет произведен его анализ
          client->connectToHost(m_params.connections.at(i).host, m_params.connections.at(i).port);

          break;
        }

        // если для данного сокета есть установленное соединение
        case QAbstractSocket::ConnectedState:
        {
          // если это текущее соединение, то ничего не делаем
          if(m_current_connection && (m_current_connection->can_entry == connection->can_entry))
            break;


          // если текущее соединение (m_current_connection) имеет меньший приоритет, чем i-ый элемент,
          // то перебрасываем текущее соединение на элемент с большим приоритетом
          // для этого, сначала отключаем сокет от слота read и обнуляем текущее соединение
          if(m_current_connection && (connection->can_entry < m_current_connection->can_entry)) {

            disconnect(m_current_connection->socket,     &QTcpSocket::readyRead,       this, &SvTcpClientMulti::read );
            m_current_connection = nullptr;
          }

          // если текущее соединение не задано, то назначаем ему текущий элемент списка
          // в данном случае текущий элемент будет иметь наибольший приоритет
          if(!m_current_connection) {

            // в качестве ткущего подключения используем текущий элемент
            m_current_connection = connection;

            // прикручиваем сокет текущего соединения к слоту read
            connect(m_current_connection->socket,     &QTcpSocket::readyRead,       this, &SvTcpClientMulti::read );

          }

          break;

         }

        // все другие состояния подключения игнорируем
        default:
          break;
      }
    }

    // пооходим по списку подключений, смотрим состояние подключения и заполняем массив
    for(int i = 0; i < m_params.connections.count(); i++) {

      if(m_params.connections.at(i).socket->state() == QAbstractSocket::ConnectedState)
        states[i] = m_params.connections.at(i).can_entry == m_current_connection->can_entry ? 2 : 1;

      else
        states[i] = 0;

    }
  }

  // отправляем массив с состояниями в протокольный модуль, для назначения значений сигналам
  p_io_buffer->state->mutex.lock();
  p_io_buffer->state->setData(&states[0], m_params.connections.count());
  p_io_buffer->state->mutex.unlock();

  qDebug() << int(states[0]) << int(states[1]) << int(states[2]) << int(states[3]);
  emit p_io_buffer->notice(p_io_buffer->state);

  // запускаем таймер проверки подключений, если не был вызван слот stop
  if(p_is_active)
    m_connectionCheckTimer->start(m_params.reconnect_period);

}

void SvTcpClientMulti::read()
// Получение данных из сокета.
{
  if(!m_current_connection)
    return;

    m_gap_timer->stop();

    p_io_buffer->input->mutex.lock();

   // Если нам надо читать данные от сокета в буфер, а протокольная часть ещё не прочла
   // прошлое содержание буфера, то сбрасываем флаг "is_ready", при этом данные,
   // получаемые от сокета будут добавляться к тем данным, которые уже имеются в буфере:
   if(p_io_buffer->input->isReady())
       p_io_buffer->input->is_ready = false;

   // Если места в буфере на новые данные от сокета нет, то очищаем его содержимое и
   // сбрасываем флаг "is_ready":
   if(p_io_buffer->input->offset + m_current_connection->socket->bytesAvailable() > p_config->bufsize)
        p_io_buffer->input->reset();


   qint64 readed = m_current_connection->socket->read(&p_io_buffer->input->data[p_io_buffer->input->offset], p_config->bufsize - p_io_buffer->input->offset);

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
  if(!buffer->isReady() || !m_current_connection)
  {
    // Если данных для передачи нет, то нечего и передавать (выходим из функции):
    return;
  }

  if (m_current_connection->socket->state() != QAbstractSocket::ConnectedState)
  {
      // Если соединения с сервером не установлено, то некому и передавать (выходим из функции):
      return;
  }

  buffer->mutex.lock();

  bool written = m_current_connection->socket->write((const char*)&buffer->data[0], buffer->offset) > 0;
  m_current_connection->socket->flush();

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
  for(auto c: m_params.connections) {

    if(c.socket) {

      if(c.socket->state() == QAbstractSocket::ConnectedState)
        c.socket->disconnectFromHost();

      delete c.socket;

    }
  }

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
