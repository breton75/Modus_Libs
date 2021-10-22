#ifndef ZN_K1_H
#define ZN_K1_H

#include <QDataStream>

#include "zn_writer_global.h"

#include "../../../../APAK/global_defs.h"

#include "../../../../../svlib/Network/Tcp/Client/1.0/sv_tcp_client.h"
#include "../../../../../svlib/SvException/svexception.h"
#include "../../../../../svlib/SvCRC/1.1/sv_crc.h"

#include "../../../../../Modus/global/storage/sv_abstract_storage.h"
#include "../../../../../Modus/global/dbus/sv_dbus.h"

#include "zn_writer_defs.h"

extern "C" {

    ZN_K1SHARED_EXPORT modus::SvAbstractStorage* create();

//    VIRTUAL_DEVICESHARED_EXPORT QString defaultDeviceParams();
//    VIRTUAL_DEVICESHARED_EXPORT QString defaultIfcParams(const QString& ifc);
//    VIRTUAL_DEVICESHARED_EXPORT QList<QString> availableInterfaces();

}

namespace zn1 {

  enum ReplyCode {

    Success = 0,
    Failure,
    NotSupportedCmd,
    WrongZoneName,
    WrongPassword,
    AlreadyInUse
  };

  const QMap<ReplyCode, QString> ReplyCodeMap = {{Success,          "Успех"                       },
                                                 {Failure,          "Отказ ЗН-К1"                 },
                                                 {NotSupportedCmd,  "Команда не поддерживается"   },
                                                 {WrongZoneName,    "Неверное имя зоны данных"    },
                                                 {WrongPassword,    "Неправильный пароль"         },
                                                 {AlreadyInUse,     "Устройство уже используется" }};

  struct AuthorizeRequest
  {
    AuthorizeRequest(QString zone_name = QString(), QString pass = QString(), quint16 cmd = CMD_CONNECT, quint16 access_code = ACC_CODE_WRITE):
      cmd(cmd),
      zone_name(zone_name),
      pass(pass),
      access_code(access_code)
    {
      length = 4 + zone_name.length() + 4 + pass.length() + 2;
    }

    QByteArray toByteArray()
    {
      QByteArray result;

      QDataStream stream(&result, QIODevice::ReadWrite);
      stream.setByteOrder(QDataStream::LittleEndian); // !
      stream << length
             << cmd
             << static_cast<quint32>(zone_name.length());

      stream.writeRawData(zone_name.toStdString().c_str(), zone_name.length());

      stream << static_cast<quint32>(pass.length());

      stream.writeRawData(pass.toStdString().c_str(), pass.length());

      stream << access_code;

      return result;

    }

    quint32     length;
    quint16     cmd;
    QString     zone_name;
    QString     pass;
    quint16     access_code;

  };

  struct AuthorizeReply
  {
    AuthorizeReply():
      length(0),
      reply_code(0),
      request_code(0),
      result(0),
      additional(QByteArray())
    {    }

    static AuthorizeReply parse(const QByteArray& ba)
    {
      AuthorizeReply result;

      if(ba.size() < 10)
        return result;

      QDataStream stream(ba);
      stream.setByteOrder(QDataStream::LittleEndian); //!

      stream >> result.length
             >> result.reply_code
             >> result.request_code
             >> result.result;

      result.additional = ba.right(result.length - 10); // согласно протокола

      return result;

    }

    quint32     length;
    quint16     reply_code;
    quint16     request_code;
    quint16     result;
    QByteArray  additional;

  };

  struct WriteRequest
  {
    WriteRequest(const QByteArray& data):
      cmd(CMD_WRITE),
      data(data)
    {
      length = data.length();
    }

    QByteArray toByteArray()
    {
      QByteArray result;

      QDataStream stream(&result, QIODevice::ReadWrite);
      stream.setByteOrder(QDataStream::LittleEndian); // !
      stream << length << cmd;

      result.append(data);

      return result;

    }

    quint32     length;
    quint16     cmd;
    QByteArray  data;

  };

  struct WriteReply
  {
    WriteReply():
      length(0),
      reply_code(0),
      request_code(0),
      result(0),
      additional(QByteArray())
    {    }

    static WriteReply parse(const QByteArray& ba)
    {
      WriteReply result;

      if(ba.length() < 10)
        return result;

      QDataStream stream(ba);
      stream.setByteOrder(QDataStream::LittleEndian); //!

      stream >> result.length
             >> result.reply_code
             >> result.request_code
             >> result.result;

      result.additional = ba.right(result.length - 10); // согласно протокола

      return result;

    }

    quint32     length;
    quint16     reply_code;
    quint16     request_code;
    quint16     result;
    QByteArray  additional;

  };

  struct BanchHeader {

    const char    marker[4] = {char(0xAA), char(0xAA), char(0xAA), char(0xAA)}; // 0xAAAAAAAA

    BanchHeader(qint64 coarseDateTime = 0):
      coarseDateTime(coarseDateTime)
    {

    }

    qint64  coarseDateTime;

    QByteArray toByteArray(quint32 dataLength)
    {
      QByteArray result(&marker[0], sizeof(marker));

      QDataStream stream(&result, QIODevice::WriteOnly);
      stream.setByteOrder(QDataStream::LittleEndian);

      stream << coarseDateTime
             << dataLength;

      stream << crc::crc16ccitt(result);

      return result;

    }

    quint32 length()
    {
      return sizeof(marker) + sizeof(coarseDateTime) + sizeof(quint32) + sizeof(quint16);
    }

  };

  struct Record
  {
    Record():
      dateTime(0),
      protocolIdentifier(QString()),
      data(QByteArray())
    {

    }

    Record(qint64  dateTime, const QString& protocolIdentifier, const QByteArray& data):
      dateTime(dateTime),
      protocolIdentifier(protocolIdentifier),
      data(data)
    {

    }

    qint64      dateTime;
    QString     protocolIdentifier;
    QByteArray  data;

    QByteArray toByteArray()
    {
      QByteArray result = QByteArray();

      if((dateTime <= 0) || protocolIdentifier.isNull() || protocolIdentifier.isEmpty())
        return result;

      QDataStream stream(&result, QIODevice::WriteOnly);
      stream.setByteOrder(QDataStream::LittleEndian);

      stream << dateTime << static_cast<quint16>(protocolIdentifier.length());
      stream.writeRawData(protocolIdentifier.toStdString().c_str(), protocolIdentifier.length());
      stream << static_cast<quint16>(data.length());

      result.append(data);

      return result;

    }
  };

  class Bunch
  {

  public:
    enum States {
      Undefined,
      Underway,
      Undelivered,
      Delivered,
      Ready
    };

    Bunch(qint64 coarseDateTime = 0, States state = Underway):
      m_header(BanchHeader(coarseDateTime)),
      m_data(QByteArray()),
      m_state(state),
      m_record_count(0)
    {

    }

    const BanchHeader header()  const { return m_header; }
    const QByteArray& data()    const { return m_data;   }
    States      state()               { return m_state;  }

    void setState(States state)
    {
      m_state = state;
    }

    void appendRecord(const QByteArray& record)
    {
      m_data.append(record);
      m_record_count++;
    }

    QByteArray toByteArray()
    {
      QByteArray result;

      result.append(m_header.toByteArray(m_data.length())).append(m_data);

      return result;
    }

    quint32 length()
    {
      return quint32(m_header.length() + m_data.length());
    }

    quint16 recordCount()
    {
      return m_record_count;
    }

  private:
    BanchHeader  m_header;
    QByteArray   m_data;
    States       m_state;

    quint16 m_record_count;

  };


  class ZNWriter: public modus::SvAbstractStorage
  {

    Q_OBJECT

    struct ZNstate {
      ZNstate(int c = 0, int a = 0, int w = 0):
        c(c), a(a), w(w)
      {}

//      quint8 state() { return quint8(c + (a << 1) + (w << 2)); }
      quint8 state() { return c & a & w; }

      quint8 c;
      quint8 a;
      quint8 w;
    };

  public:
    ZNWriter();

    bool configure(modus::StorageConfig* config) override;
    bool bindSignal(modus::SvSignal* signal, modus::SignalBinding binding) override;

  private:

    QQueue<zn1::Bunch*>    bunches;

    sv::tcp::Client*  m_socket;
    zn1::Params       m_params;
    bool              m_authorized;

    modus::SvSignal* m_state_signal;

//    QMap<QString, modus::SvSignal*> m_zn_state;
    ZNstate m_zn_state;

//    QTimer* m_timer = nullptr;

//    void setState(int doChangeFlags, const QString& writeState = STATE_OK, const QString& authorization = STATE_OK, const QString& connectionState = STATE_OK);
    void setState(int writeState, int authorization, int connectionState);

//  private slots:
//    void checkupSignals();

  public slots:
    void start() override;
    void signalUpdated(modus::SvSignal* signal) override;
    void signalChanged(modus::SvSignal* signal) override
    {
      Q_UNUSED(signal);
    }

    void write();

  };
}

#endif // ZN_K1_H
