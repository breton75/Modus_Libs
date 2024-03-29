﻿#ifndef SV_TCP_CLIENT_H
#define SV_TCP_CLIENT_H

#include <QNetworkInterface>
#include <QTcpSocket>

#include "tcp_client_global.h"
#include "tcp_client_defs.h"

#include "../../../../Modus/global/global_defs.h"
#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

#define LIB_SHORT_INFO \
  "TCP клиент. Интерфейсная библиотека Modus. Версия" LIB_VERSION "\n"

#define LIB_DESCRIPTION \
  LIB_SHORT_INFO \
  "Алгоритм работы:\n"\
  "  1. Подключение к серверу с заданными праметрами " P_HOST " и " P_PORT ".\n"\
  "  2. При получении новых данных, они помещаются в буфер input и эмитируется сигнал dataReaded.\n"\
  "  3. При получении сигнала readyWrite от протокольной библиотеки, буфер output помещается в tcp стек на отправку.\n"\
  "  4. С интервалом, заданным параметром " P_RECONNECT_PERIOD " проверяется соединение с сервером. При отсутствии подключения, производится попытка переподкючения.\n"\
  "Автор " LIB_AUTHOR


extern "C" {

    TCP_CLIENT_EXPORT modus::SvAbstractInterface* create();

    TCP_CLIENT_EXPORT const char* getVersion();
    TCP_CLIENT_EXPORT const char* getParams();
    TCP_CLIENT_EXPORT const char* getInfo();
    TCP_CLIENT_EXPORT const char* getDescription();
}

class SvTcpClient: public modus::SvAbstractInterface
{
public:
  SvTcpClient();
  ~SvTcpClient() override;

  // Получение параметров интерфейса из конфигурационного файла устрйства:
  virtual bool configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer) override;

public slots:
  // Создание и инициализация объектов; создание подключений сигналов к слотам,
  // которые необходимы для работы tcp-клиента:
  bool start() override;

  // Получение данных из сокета:
  void read() override;

  // Запись данных в сокет:
  void write(modus::BUFF* buffer) override;

  void stop() override;

private:
  // TCP-сокет клиента
  QTcpSocket*   m_client = nullptr;

 // Структура, хранящая параметры TCP-клиента:
  tcp::Params   m_params;

  // Таймер, используемый в функции "SvTcpClient::read". Коментарии - в этой функции.
  QTimer*       m_gap_timer;

  // Таймер, пр таймауту которого, мы выполняем проверку установлено ли TCP-соединение
  // с сервером:
  QTimer*       m_connectionCheckTimer;

  // Флаг, говорящий о том, что выполняется команда разрыва TCP-соединения
  // с сервером. Этот флаг нужен нам для того, чтобы с момента подачи TCP-сокету команды
  // на разъединение соединения с сервером до момента, когда соединение будет разорвано, не
  // выдавать TCP-сокету команды на попытку соединиться с сервером:
//  bool breakConnectionCommand;

  void datalog(const QByteArray& bytes, QString& message);

  // На момент запуска TCP-клиента, TCP-сервер может быть не запущен, поэтому
  // мы по таймеру будем c интервалом, указанным в параметре интерфейса, вызывать
  // функцию "checkConnection", которая
  // будет проверять установлено ли TCP-соединение и, если оно не установлено, пытаться
  // его установить:
   void checkConnection(void);


private slots:
  // Отображение в утилите "logview" ошибки сокета:
  void socketError(QAbstractSocket::SocketError err);

  // Отображение в утилите "logview" изменившегося состояния подключения:
  void stateChanged(QAbstractSocket::SocketState state);

  // После выполнения чтения из сокета испускаем сигнал "dataReaded" с некоторой задержкой.
  // Более подробное описание см. в функции "SvTcpClient::read":
  void newData(void);

  // По подключению к серверу -> отображаем информацию об этом
  // в утилите "logview:
  void connected(void);

  // По отключению от сервера -> отображаем информацию об этом
  // в утилите "logview":
  void disconnected(void);

  // Выяснение, какую команду требуется выполнить, и её выполнение.
  // Аргумент "command" - требуемая команда.
  // Возможные команды: "breakConnection" - разорвать соединение с сервером.
  void say_WorkingOut(QByteArray command);


  void emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type);


};

#endif // SV_TCP_CLIENT_H
