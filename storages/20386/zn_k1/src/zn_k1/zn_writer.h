#ifndef ZN_K1_H
#define ZN_K1_H

#include "zn_writer_global.h"

#include "../../../../../../svlib/Network/Tcp/Client/1.0/sv_tcp_client.h"
#include "../../../../../../svlib/SvException/svexception.h"

#include "../../../../../../Modus/global/storage/sv_abstract_storage.h"
#include "../../../../../../Modus/global/dbus/sv_dbus.h"

#include "zn_writer_defs.h"

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

  struct ConnectRequest
  {
    ConnectRequest(QString zone_name = QString(), QString pass = QString(), quint16 cmd = CMD_CONNECT, quint16 access_code = ACC_CODE_WRITE):
      zone_name(zone_name),
      pass(pass),
      cmd(cmd),
      access_code(access_code)
    {
      length = 4 + zone_name.length() + 4 + pass.length() + 2;
    }

    QByteArray toByteArray()
    {
      QByteArray result;

      QDataStream stream(result);
      stream << length
             << cmd
             << static_cast<quint32>(zone_name.length())
             << zone_name.toStdString().c_str()
             << static_cast<quint32>(pass.length())
             << pass.toStdString().c_str()
             << access_code;

      return result;

    }

    quint32     length;
    quint16     cmd;
    QString     zone_name;
    QString     pass;
    quint16     access_code;

  };

  struct ConnectReply
  {
    ConnectReply():
      length(0),
      reply_code(0),
      request_code(0),
      result(0),
      additional(QByteArray())
    {    }

    static ConnectReply parse(const QByteArray& ba)
    {
      ConnectReply result;

      if(ba.size() < 10)
        return result;

      QDataStream stream(ba);
      stream >> result.length
             >> result.reply_code
             >> result.request_code
             >> result.result;

      result.additional = ba.right(result.length - 4); // согласно протокола

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
      data(data),
      cmd(CMD_WRITE)
    {
      length = data.length();
    }

    QByteArray toByteArray()
    {
      QByteArray result;

      QDataStream stream(result);
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

      if(ba.size() < 10)
        return result;

      QDataStream stream(ba);
      stream >> result.length
             >> result.reply_code
             >> result.request_code
             >> result.result;

      result.additional = ba.right(result.length - 4); // согласно протокола

      return result;

    }

    quint32     length;
    quint16     reply_code;
    quint16     request_code;
    quint16     result;
    QByteArray  additional;

  };

  class ZNWriter: public modus::SvAbstractStorage
  {

    Q_OBJECT

  public:
    ZNWriter();

    virtual bool configure(modus::StorageConfig* config) override;

    virtual bool bindSignal(modus::SvSignal* signal) override;

    virtual void processSignals() override;

  private:
    QQueue<modus::SvSignal*> m_queue;

    sv::tcp::Client* m_socket;

    zn1::Params m_params;

  protected:
    void run() override;

  private slots:
    void signalUpdated(modus::SvSignal* signal);

  };
}

#endif // ZN_K1_H
