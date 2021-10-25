#include "sv_tcp_client.h"

/*******************************************************
 *                      Client                    *
 * *****************************************************/
tcp::Client::Client() :
  m_host("localhost"),
  m_port(35580),
  m_flags(DEFAULT_FLAGS)
{

}

tcp::Client::Client(const QString& host, quint16 port, int flags) :
  m_host(host),
  m_port(port),
  m_flags(flags)
{
  connect(this, &QAbstractSocket::error,        this, &tcp::Client::socketError);
  connect(this, &QAbstractSocket::stateChanged, this, &tcp::Client::stateChanged);
}

tcp::Client::~Client()
{
//  disconnectFromHost();
//  delete _socket;
  deleteLater();
}

bool tcp::Client::connectTo(const QString& host, quint16 port)
{
  QHostAddress addr;

  if(host.toLower() == "localhost")
    addr = QHostAddress(QHostAddress::LocalHost);

  else if (QHostAddress(host).toIPv4Address() == 0) {

    m_last_error = QString("Неверный IP адрес: %1").arg(host);
    return false;
  }
  else
    addr = QHostAddress(host);

  return connectTo(addr, port);
  
}

bool tcp::Client::connectTo(const QHostAddress& host, quint16 port, int timeout)
{
  if (state() != QAbstractSocket::ConnectedState) {

    connectToHost(host, port);

    if (!waitForConnected(timeout)) {

      m_last_error = errorString();
      return false;
    }
  }

  return true;
}

/*void tcp::Client::disconnectFromHost()
{
  _socket->disconnectFromHost();
  _socket->close();
  _connected  = false;
}*/

void tcp::Client::socketError(QAbstractSocket::SocketError err)
{
  emit message(tcp::SocketErrors.value(err, QString("Неизвестная ошибка подключения")), lldbg, mterr);
}

void tcp::Client::stateChanged(QAbstractSocket::SocketState state)
{
  emit message(tcp::SocketStates.value(state, QString("Неизвестное состояние подключения")), lldbg, sv::log::mtConnection);
}

QByteArray tcp::Client::readAll()
{
  QByteArray result;

  result = static_cast<QTcpSocket*>(this)->readAll();

  if(m_flags & tcp::LogInData) {

    if(m_flags & tcp::InDataAsHex)
      emit message(QString(result.toHex()), lldbg, sv::log::mtReceive);

    else
      emit message(QString(result), lldbg, sv::log::mtReceive);
  }

  return result;
}

quint64 tcp::Client::write(const QByteArray& data)
{
  quint64 result = static_cast<QTcpSocket*>(this)->write(data);
  this->flush();

  if(m_flags & tcp::LogOutData) {

    if(m_flags & tcp::OutDataAsHex)
      emit message(QString(data.toHex()), lldbg, sv::log::mtSend);

    else
      emit message(QString(data), lldbg, sv::log::mtSend);
  }

  return result;

}

 
