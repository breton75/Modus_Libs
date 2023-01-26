#ifndef SV_UDP_H
#define SV_UDP_H

#include <QNetworkInterface>
#include <QUdpSocket>

#include "ifc_udp_global.h"
#include "udp_defs.h"

#include "../../../../Modus/global/global_defs.h"
#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"

#define LIB_SHORT_INFO \
  "UDP сервер, слушающий один порт. Реализована возможность перенаправления данных на другой узел сети. Интерфейсная библиотека Modus. Версия" LIB_VERSION "\n"

#define LIB_DESCRIPTION \
  LIB_SHORT_INFO \
  "Алгоритм работы:\n"\
  "1. Проверяем, указан ли в конфигурации параметр 'ifc'. Если указан, то при запуске UdpSocket'а, указываем в качестве адреса, тот, который назначен этому интерфейсу. Примечание: интерфейсу могут быть назначены несколько ip адресов. Мы берем первый."\
  "  Если параметр 'ifc' не указан, то указывается QHostAddress::Any.\n"\
  "<b>2. Получение данных.</b>\n"\
  "  Данные, которые приходят на порт, должны быть помещены в буфер обмена modus::IOBuffer. Это происходит в слоте read. Перед запуском на прослушивание порта сокетом, привязываем сигнал QUdpSocket::readyRead к слоту read. "\
  "При получении данных, они помещаются в конец буфера (добавляются). Если размер данных уже помещенных в буфер и размер новой порции данных в сумме превышают заданный размер буфера, то указатель записи передвигается в начало."\
  "После помещения данных в буфер, испускается сигнал modus::IOBuffer::dataReaded, сигнализирующий протокольной библиотеке, что данные готовы к обработке.\n"\
  "Здесь есть важный момент. Так как данные по сети могут передаваться отдельными частями, между которыми есть временной интервал - gap (это можно увидеть в сниффере), то сигнал испускается не сразу после получения данных, а с некоторой задержкой. Эта задержка определяет время, в течение которого мы ждем другие части пакета."\
  "То есть, получив данные, кладем их в буфер, после чего запускается gap-таймер. Если в течение работы таймера, будут получены еще данные, то таймер останавливается, данные добавляются в конец буфера и таймер запускается снова. Для gap-таймера задается очень короткий интервал времени, по умолчанию 10 мс. На практике он гораздо меньше."\
  "К сигналу timeout gap-таймера привязан слот, в котором и испускается сигнал dataReaded.\n"\
  "3. Отправка данных.\n"\
  "  Отправка данных производится в слоте write, который вызывается по сигналу modus::IOBuffer::readyWrite. При отправке, даные находящиеся в буфере записываются в udp сокет.\n"\
  "4. Синхронизация потоков интерфейсной и протокольной частей производится при помощи мьютексов.\n"\
  "5. Перенаправление данных (forwarding).\n"\
  "  На практике встречаются ситуации, когда один и тот же поток данных, необходимо обрабатывать в нескольких местах. Данная библиотека позволяет, отправить копию полученных данных на другой узел. Для этого служит парметр forwarding."\
  "Если в конфигурации указаны параметры перенаправления, то после получения данных от внешней системы, эти данные будут отправлены на заданный узел:порт. Если в конфигурации параметры перенаправления не заданы, то данные никуда не отправляются.\n"\
  "Автор " LIB_AUTHOR


extern "C" {

    IFC_UDP_EXPORT modus::SvAbstractInterface* create();

    IFC_UDP_EXPORT const char* getVersion();
    IFC_UDP_EXPORT const char* getInfo();
    IFC_UDP_EXPORT const char* getParams();
    IFC_UDP_EXPORT const char* getDescription();
}

class SvUdp: public modus::SvAbstractInterface
{
public:
  SvUdp();
  ~SvUdp() override;

  virtual bool configure(modus::DeviceConfig* config, modus::IOBuffer*iobuffer) override;

public slots:
  bool start() override;
  void read() override;
  void write(modus::BUFF* buffer) override;

private:
  QUdpSocket*   m_socket;
  QUdpSocket*   m_forward_socket;

  udp::Params     m_params;

  QTimer*       m_gap_timer;

  QTimer*       m_test_timer;

private slots:
  void newData();
  void emit_message(const QByteArray& bytes, sv::log::Level level, sv::log::MessageTypes type);


};

#endif // SV_UDP_H
