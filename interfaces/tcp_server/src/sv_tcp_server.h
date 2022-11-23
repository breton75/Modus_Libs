#ifndef SV_TCP_SERVER_H
#define SV_TCP_SERVER_H

#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTcpServer>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "tcp_server_global.h"
#include "tcp_server_defs.h"

#include "../../../../Modus/global/global_defs.h"
#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

#define LIB_SHORT_INFO \
  "TCP сервер (для версии 1.0.0 возможность множественных подключений не реализована). Интерфейсная библиотека Modus. Версия" LIB_VERSION "\n"

#define LIB_DESCRIPTION \
  LIB_SHORT_INFO \
  "Алгоритм работы:\n"\
  "  1. Запускается tcp сервер на прослушивание заданного порта на заданном сетевом интерфейсе. Для указания сетевого интерфейса используется "\
  "ip адрес этого интерфейса. казать адрес интерфейса возможно двумя способами:\n    1) указать реальный ip адрес назначенный сетвому ннтерфейсу "\
  "на локальной машине;\n    2) указать имя сетевого интерфейса. Тогда будет использован ip адрес назначенный интерфейсу с этим именем.\n"\
  "  В случае если сетевому интерфейсу назначено несколько ip адресов, будет использован первый.\n"\
  "  Одновременное использование параметров " P_IFC_NAME " и " P_TCP_LISTEN_ADDRESS " не имеет смысла, т.к. в этом случае будет использован параметр " P_TCP_LISTEN_ADDRESS ".\n"\
  "  2. При подключении клиента, его сокет помещается в список подключений и подключается к слоту read. Список представляет собой список пар 'ключ:значение', "\
  "где в качестве ключа используется socket descriptor (идентификатор сокета), а в качестве значения - указатель на сокет.\n"\
  "  При получении новых данных, в буфер input помещается идентификатор сокета, затем помещаются полученные данные и эмитируется сигнал dataReaded. "\
  "Таким образом протокольная библиотека может различать данные от разных клиентов. \n"\
  "  При отключении клиента, его сокет отключается от слотов и удаляется из списка.\n"\
  "  3. Для контроля состояния подключения, назначаем каждому новому сокету параметры keep alive.\n"\
  "  4. При формировании пакета данных для клиента, протокольная библиотека должна поместить в начало буфера output идентификатор клиента (сокета). "\
  "Если идентификатор равен 0, то пакет данных отправляется всем клиентам из списка.\n"\
  "  5. При получении сигнала readyWrite от протокольной библиотеки, буфер output помещается в tcp стек на отправку того сокета, чей идентификатор задан в начале буфера. "\
  "Либо, если идентификатор равен 0, то всем сокетам из списка.\n"\
  "Автор " LIB_AUTHOR


extern "C" {

    TCP_SERVER_EXPORT modus::SvAbstractInterface* create();

    TCP_SERVER_EXPORT const char* getVersion();
    TCP_SERVER_EXPORT const char* getParams();
    TCP_SERVER_EXPORT const char* getInfo();
    TCP_SERVER_EXPORT const char* getDescription();
}

class SvTcpServer: public modus::SvAbstractInterface
{
    Q_OBJECT

public:
  SvTcpServer();

  // Получение параметров интерфейса из конфигурационного файла устрйства.
  virtual bool configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer) override;

public slots:
  // Создание и инициализация объектов; создание подключений сигналов к слотам,
  // которые необходимы для работы tcp-сервера:
  bool start() override;

  // Получение данных от сокета клиентского подключения:
  void read() override;

  // Запись данных в сокет клиентского подключения:
  void write(modus::BUFF* buffer) override;

private:
//  QPointer <QTcpSocket> m_clientConnection = nullptr;
  QTcpSocket* m_clientConnection = nullptr;
  QTcpServer* m_tcpServer = nullptr;

  // Структура, хранящая параметры TCP-сервера:
  tcp::Params m_params;

  // Таймер, который используется при операции чтения данных от клиента.
  // Коментарии - в функции: SvTcpServer::read.
  QTimer*  m_gap_timer;

  void datalog(const QByteArray& bytes, QString& message);

private slots:
  // Отображение в утилите "logview" ошибки сокета:
  void socketError(QAbstractSocket::SocketError err);

  // Отображение в утилите "logview" изменившегося состояния подключения:
  void stateChanged(QAbstractSocket::SocketState state);

  // После выполнения чтения из сокета клиента испускаем сигнал "dataReaded" с некоторой задержкой.
  // Более подробное описание см. в функции "SvTcpServer::read":
  void newData();

  // Функция вызывается, когда серверу доступно новое соединение с клиентом:
  void newConnection();

  // По отключению клиента -> отображаем информацию об этом в утилите "logview" и уничтожаем его TCP-сокет:
  void disconected();

  void emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type);
};

#endif // SV_TCP_SERVER_H
