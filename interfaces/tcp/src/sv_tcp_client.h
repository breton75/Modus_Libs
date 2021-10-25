#ifndef SV_TCP_CLIENT_H
#define SV_TCP_CLIENT_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>
#include <QHostAddress>

#include "tcp_defs.h"

#include "../../../../svlib/SvAbstractLogger/svabstractlogger.h"
#include "../../../../Modus/global/global_defs.h"
#include "../../../../Modus/global/device/device_defs.h"

namespace tcp {

  class Client : public QTcpSocket
  {
    Q_OBJECT

  public:
    explicit Client();

    explicit Client(const QString& host, quint16 port, int flags = DEFAULT_FLAGS);

    ~Client();

    void            setHost(const QString& host)    { m_host   = host;   }
    void            setPort(quint16 port)           { m_port   = port;   }
    void            setFlags(int flags)             { m_flags  = flags;  }

    int             flags() const                   { return m_flags;    }
    const QString   host()  const                   { return m_host;     }
    quint16         port()  const                   { return m_port;     }


    QByteArray      readAll();


    bool            connectTo(const QString& host, quint16 port);
    bool            connectTo(const QHostAddress& host, quint16 port, int timeout = DEFAULT_TIMEOUT);

    const QString& lastError() const { return m_last_error; }

  private:
    QString       m_host;
    quint16       m_port;
    int           m_flags;
    QString       m_last_error = "";

  signals:
    void message(const QString msg, int level = sv::log::llDebug, int type  = sv::log::mtDebug);

  private slots:
    void socketError(QAbstractSocket::SocketError err);
    void stateChanged(QAbstractSocket::SocketState state);

  public slots:
    quint64         write(const QByteArray &data);

  };
}


#endif // SV_TCP_CLIENT_H








