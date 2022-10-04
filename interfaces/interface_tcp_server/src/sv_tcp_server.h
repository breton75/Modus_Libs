#ifndef SV_TCP_SERVER_H
#define SV_TCP_SERVER_H

#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTcpServer>

#include "tcp_server_global.h"
#include "tcp_server_defs.h"

#include "../../../../Modus/global/global_defs.h"
#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

extern "C" {

    TCP_SERVER_EXPORT modus::SvAbstractInterface* create();

    TCP_SERVER_EXPORT const char* getDefaultParams();
    TCP_SERVER_EXPORT const char* getName();
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
  QPointer <QTcpSocket> m_clientConnection = nullptr;
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
