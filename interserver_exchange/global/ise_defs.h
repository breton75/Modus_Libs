#ifndef ISE_DEFS_H
#define ISE_DEFS_H

#include <QtGlobal>
#include <QDataStream>

#define E_IMPERMISSIBLE_VALUE "Недопустимое значение параметра %1: %2.\n%3"
#define E_NO_PARAM  "Не задан обязательный параметр \"%1\""

#define MAX_PACKET_SIZE 0xFFFF

// имена параметров устройств
#define P_ISE_ISEID           "iseid"
#define P_ISE_SENDER_ISEID    "sender_iseid"
#define P_ISE_RECEIVER_ISEID  "receiver_iseid"
#define P_ISE_SEND_INTERVAL   "send_interval"
#define P_ISE_RESET_TIMEOUT   "reset_timeout"
#define P_ISE_HOST            "host"
#define P_ISE_PORT            "port"

#define ISE_DEFAULT_RESET_INTERVAL  10
#define ISE_DEFAULT_SEND_INTERVAL   1000
#define ISE_DEFAULT_ISEID           0
#define ISE_DEFAULT_SENDER_ISEID    0
#define ISE_DEFAULT_RECEIVER_ISEID  0
#define ISE_DEFAULT_PORT            25555

namespace ise {

  const QByteArray DEF_SIGN = QByteArray("ISE");

  #pragma pack(push,1)
  struct Header
  {
    char    sign[3];
    quint16 sender;
    quint16 receiver;
    quint16 data_length;
  };
  #pragma pack(pop)

}

#endif // ISE_DEFS_H




