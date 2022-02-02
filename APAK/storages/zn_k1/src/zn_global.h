#ifndef ZN_GLOBAL
#define ZN_GLOBAL

#include <QDataStream>

#include "../../../../../svlib/SvCRC/1.1/sv_crc.h"


#define DEFAULT_DATETIME_FORMAT   "yyyy-MM-dd hh:mm:ss"

#define P_FULL_FILE_NAME          "full_file_name"
#define P_TASKS                   "tasks"
#define P_MARKER                  "marker"
#define P_BEGIN                   "begin"
#define P_END                     "end"
#define P_FILE_NAME               "file_name"

/// запись данных на устройство
#define P_ZN_MARKER               "zn_marker"
#define P_HOST                    "host"
#define P_PORT                    "port"
#define P_ZONE                    "zone"
#define P_PASS                    "pass"
#define P_QUEUE_LEN               "queue_len"
#define P_WRITE_BUF               "write_buf"

/// чтение данных с устройства
#define P_HOST                    "host"
#define P_PORT                    "port"
#define P_ZONE                    "zone"
#define P_PASS                    "pass"
#define P_QUEUE_LEN               "queue_len"
#define P_ASK_ZONE_SIZE           "ask_zone_size"
#define P_START_BYTE              "start_byte"
#define P_ZONE_SIZE_GB            "zone_size_gb"
#define P_ZONE_SIZE               "zone_size_mb"
#define P_FULL_FILE_NAME          "full_file_name"
#define P_BUFF_SIZE               "buff_size_mb"

///дефолтные значения
#define DEFAULT_PORT              10000
#define DEFAULT_INTERVAL          1000
#define DEFAULT_QUEUE_LEN         1000
#define DEFAULT_WRITE_BUFFER_LEN  102400  // bytes. 100 kB

#define DEFAULT_READ_BUFFER_LEN   16      // megabytes. 16 MB
#define DEFAULT_READ_INTERVAL     5       // 1 сек.
#define DEFAULT_ZONE_SIZE_MB      3840    // 3.75 Gb

/// команды для работы с устройством
#define CMD_CONNECT               200
#define CMD_ANSWER                100
#define CMD_WRITE                 501
#define ACC_CODE_WRITE            2
#define CMD_READ                  502
#define ACC_CODE_READ             3
#define CMD_SIZE                  503

/// служебные определения
#define TAG_CONNECTION_STATE      "c"
#define TAG_AUTHORIZATION         "a"
#define TAG_WRITE_STATE           "w"

#define STATE_NO_CONNECTION       0
#define STATE_NO_AUTHORITY        0
#define STATE_NO_WRITING          0
#define STATE_NO_READING          0

#define STATE_CONNECTION_OK       1
#define STATE_AUTHORITY_OK        1
#define STATE_WRITING_OK          1
#define STATE_READING_OK          1

#define STATE_OK                  "OK"

namespace zn1 {

  /// общие. структура записи
  const char  BunchMarker[4] = {char(0xAA), char(0xAA), char(0xAA), char(0xAA)};

  struct BunchHeader {

    char  marker[sizeof(zn1::BunchMarker)]; // = {char(0xAA), char(0xAA), char(0xAA), char(0xAA)}; // 0xAAAAAAAA
    qint64      coarseDateTime;
    quint32     dataLength;
    quint16     crc16;

    explicit BunchHeader():
      marker{*zn1::BunchMarker}, //[0], sizeof(ZNMarker)),
      coarseDateTime(coarseDateTime),
      dataLength(0),
      crc16(0)
    {

    }

    explicit BunchHeader(qint64 coarseDateTime):
      marker{*zn1::BunchMarker},
      coarseDateTime(coarseDateTime),
      dataLength(0),
      crc16(0)
    {

    }

    bool fromRawData(const char* data)
    {
      QByteArray b(data, sizeof(marker) + sizeof(qint64)  + sizeof(quint16));
      QDataStream s(b);
      s.setByteOrder(QDataStream::LittleEndian); // !

      s.readRawData(&marker[0], sizeof(marker));
      s >> coarseDateTime >> dataLength >> crc16;

      return (crc16 == crc::crc16ccitt(data, sizeof(marker) + sizeof(coarseDateTime) + sizeof(dataLength)));
    }

    QByteArray toByteArray(quint32 dataLength)
    {
      QByteArray result{};

      QDataStream stream(&result, QIODevice::WriteOnly);
      stream.setByteOrder(QDataStream::LittleEndian);

      stream.writeRawData(&marker[0], sizeof(marker));

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

  class Record
  {
  public:
    Record():
      m_date_time(0),
      m_zn_marker(QString()),
      m_record_data(QByteArray())
    {

    }

    Record(qint64  dateTime, const QString& zn_marker, const QByteArray& data):
      m_date_time(dateTime),
      m_zn_marker(zn_marker)
    {
      QDataStream stream(&m_record_data, QIODevice::WriteOnly);
      stream.setByteOrder(QDataStream::LittleEndian);

      stream << dateTime;

      stream << static_cast<quint16>(zn_marker.length());
      stream.writeRawData(zn_marker.toStdString().c_str(), zn_marker.length());

      stream << static_cast<quint16>(data.length());
      stream.writeRawData(data.data(), data.length());
    }

    const QByteArray& recordData()
    {
      return m_record_data;
    }

    void fromRawData(const char* data)
    {
      QByteArray r{data, sizeof(m_date_time) + sizeof(quint16)};
      QDataStream stream(r);
      stream.setByteOrder(QDataStream::LittleEndian); // !

      quint16 system_marker_length;
      quint16 data_length;

      //!
      stream >> m_date_time;

      //! system_marker
      stream >> system_marker_length;
      char system_marker[system_marker_length + 1];
      memset(&system_marker[0], 0, system_marker_length + 1);

      stream.readRawData(&system_marker[0], system_marker_length);

      m_zn_marker = QString(&system_marker[0]);

      //! data
      stream >> data_length;
      char record_data[data_length];

      stream.readRawData(&record_data[0], data_length);

      m_record_data = QByteArray(&record_data[0], data_length);

    }

    int length()
    {
      return m_record_data.length();
    }

  private:
    qint64      m_date_time;
    QString     m_zn_marker;
    QByteArray  m_record_data;

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
      m_header(BunchHeader(coarseDateTime)),
      m_data(QByteArray()),
      m_state(state),
      m_record_count(0)
    {

    }

    const BunchHeader header()  const { return m_header; }
    const QByteArray& data()    const { return m_data;   }
    States      state()               { return m_state;  }

    bool setHeader(const char* data)
    {
      return m_header.fromRawData(data);
    }

    void setState(States state)
    {
      m_state = state;
    }

    void appendRecord(Record* record)
    {
      m_data.append(record->recordData());
      m_record_count++;
    }

    QByteArray toByteArray()
    {
      QByteArray result;

      result.append(m_header.toByteArray(m_data.length())).append(m_data);

      return result;
    }

    void appendToStream(QDataStream* const stream)
    {
      stream->writeRawData(m_header.toByteArray(m_data.length()).data(), int(m_header.length()));
      stream->writeRawData(m_data, int(m_data.length()));
    }

    void makeByteArray(QByteArray& ba)
    {
      ba.append(m_header.toByteArray(m_data.length())).append(m_data);
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
    BunchHeader  m_header;
    QByteArray   m_data;
    States       m_state;

    quint16 m_record_count;

  };

  /// общие. работа с устройством

  enum ReplyCode {

    Success         = 0,
    Failure         = 1,
    NotSupportedCmd = 2,
    WrongZoneName   = 3,
    WrongPassword   = 4,
    AlreadyInUse    = 5,
    WrongParams     = 8
  };

  const QMap<ReplyCode, QString> ReplyCodeMap = {{Success,          "Успех"                       },
                                                 {Failure,          "Отказ ЗН-К1"                 },
                                                 {NotSupportedCmd,  "Команда не поддерживается"   },
                                                 {WrongZoneName,    "Неверное имя зоны данных"    },
                                                 {WrongPassword,    "Неправильный пароль"         },
                                                 {AlreadyInUse,     "Устройство уже используется" },
                                                 {WrongParams,      "Неверные параметры команды" }};


  /// авторизация
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

  /// запись данных
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

  /// чтение данных
  struct ReadRequest
  {
    ReadRequest(quint64 start_byte, quint64 byte_count):
      length(16),
      cmd(CMD_READ),
      start_byte(start_byte),
      byte_count(byte_count)
    {

    }

    QByteArray toByteArray()
    {
      QByteArray result;

      QDataStream stream(&result, QIODevice::ReadWrite);
      stream.setByteOrder(QDataStream::LittleEndian); // !
      stream << length << cmd << start_byte << byte_count;

      return result;

    }

    quint32 length;
    quint16 cmd;
    quint64 start_byte;
    quint64 byte_count;

  };

  struct ReadReply
  {
    ReadReply():
      failed(false),
      length(0),
      reply_code(0),
      request_code(0),
      result(0),
      start_byte(0),
      data(QByteArray())
    {    }

    static ReadReply parse(const QByteArray& ba)
    {
      ReadReply result;

      if(ba.length() < 10) {

        result.failed = true;
        return result;
      }

      QDataStream stream(ba);
      stream.setByteOrder(QDataStream::LittleEndian); //!

      stream >> result.length           // здесь общий размер запрошенных данных, а не одного пакета!
             >> result.reply_code
             >> result.request_code
             >> result.result;

      if(result.result == ReplyCode::Success) {

        if(ba.length() > 17) {

          stream >> result.start_byte;
          result.data = ba.right(ba.size() - 18); // согласно протокола
//          qDebug() << "result: length" << result.length << "data size" << result.data.size();

        }
        else
          result.failed = true;

      }

      return result;

    }

    bool        failed;
    quint32     length;
    quint16     reply_code;
    quint16     request_code;
    quint16     result;
    quint64     start_byte;
    QByteArray  data;

  };

  /// запрос размера данных
  struct DataSizeRequest
  {
    DataSizeRequest():
      cmd(CMD_SIZE)
    {
      length = 0;
    }

    QByteArray toByteArray()
    {
      QByteArray result;

      QDataStream stream(&result, QIODevice::ReadWrite);
      stream.setByteOrder(QDataStream::LittleEndian); // !
      stream << length << cmd;

      return result;

    }

    quint32     length;
    quint16     cmd;

  };

  struct DataSizeReply
  {
    DataSizeReply():
      failed(false),
      length(0),
      reply_code(0),
      request_code(0),
      result(0),
      data_size(0),
      data_pointer(0),
      additional(QByteArray())
    {    }

    static DataSizeReply parse(const QByteArray& ba)
    {
      DataSizeReply result;

      if(ba.length() < 10) { // согласно протокола

        result.failed = true;
        return result;
      }

      QDataStream stream(ba);
      stream.setByteOrder(QDataStream::LittleEndian); //!

      stream >> result.length
             >> result.reply_code
             >> result.request_code
             >> result.result;

      if(result.result == ReplyCode::Success) {

         if(ba.length() > 25) {

           stream >> result.data_size
                  >> result.data_pointer;

           result.additional = ba.right(result.length - 26); // согласно протокола

         }
         else
           result.failed = true;

      }

      return result;

    }

    bool        failed;
    quint32     length;
    quint16     reply_code;
    quint16     request_code;
    quint16     result;
    quint64     data_size;
    quint64     data_pointer;
    QByteArray  additional;

  };


  /// извлечение данных
  class TaskPeriod
  {
  public:
    explicit TaskPeriod():
      m_begin(QDateTime::currentDateTime()),
      m_end(QDateTime::currentDateTime())
    {  }

    explicit TaskPeriod(QDateTime begin, QDateTime end):
      m_begin(begin),
      m_end(end)
    {

    }

    void setBegin(QDateTime begin)
    {
      m_begin = begin;
    }

    void setEnd(QDateTime end)
    {
      m_end = end;
    }

    void setData(QDateTime begin, QDateTime end)
    {
      m_begin = begin;
      m_end   = end;

    }

    QDateTime begin() { return m_begin; }
    QDateTime end()   { return m_end; }

    bool isValid() { return !m_begin.isNull() && m_begin.isValid() && !m_end.isNull() && m_end.isValid(); }
    bool contains(QDateTime point) { return ((point >= m_begin) && (point <= m_end)); }

    bool operator==(const TaskPeriod& other) { return (other.m_begin == m_begin) && (other.m_end == m_end); }

  private:
    QDateTime m_begin;
    QDateTime m_end;

  };

  class Task {

  public:
    Task():
      m_id(0),
      m_marker(QString()),
      m_begin(QDateTime::currentDateTime()),
      m_end(QDateTime::currentDateTime()),
      m_path(QString()),
      m_file_name(QString())
    {
      m_period.setData(m_begin, m_end);
    }

    Task(Task* task):
      m_id(task->id()),
      m_marker(task->marker()),
      m_begin(task->begin()),
      m_end(task->end()),
      m_path(task->path()),
      m_file_name(task->file_name())
    {
      m_period.setData(m_begin, m_end);
    }

    Task& operator=(const Task& other)
    {
      if(this == &other)
        return *this;

      m_id = other.m_id;
      m_marker = other.m_marker;
      m_begin = other.m_begin;
      m_end = other.m_end;
      m_path = other.m_path;
      m_file_name = other.m_file_name;
      m_period.setData(m_begin, m_end);

      return *this;
    }

    qint64      id()                const { return m_id;            }
    uint        marker_hash()             { return qHash(m_marker); }

    const QString     marker()      const { return m_marker;        }
    const QString     path()        const { return m_path;          }
    const QString     file_name()   const { return m_file_name;     }
    const QDateTime   begin()       const { return m_begin;         }
    const QDateTime   end()         const { return m_end;           }

    const TaskPeriod& period()      const { return m_period;        }

    QString   save_path()
    {
      return QDir(m_path.replace("[MARKER]",  m_marker)).filePath(m_file_name
                                                                  .replace("[MARKER]",  m_marker)
                                                                  .replace("[BEGIN]",   m_begin.toString("ddMMyyyy-hhmmss"))
                                                                  .replace("[END]",     m_end.toString("ddMMyyyy-hhmmss"))
                                                                .append(m_file_name.endsWith(".znr") ? "" : ".znr"));
    }

    void setId(qint64 id)
    {
      m_id    = id;
    }

    void setBegin(QDateTime begin)
    {
      m_begin   = begin;
      m_period  = TaskPeriod(m_begin, m_end);
    }

    void setEnd(QDateTime end)
    {
      m_end     = end;
      m_period  = TaskPeriod(m_begin, m_end);
    }

    void setPeriod(QDateTime begin, QDateTime end)
    {
      m_begin   = begin;
      m_end     = end;
      m_period  = TaskPeriod(m_begin, m_end);
    }

    void setPeriod(TaskPeriod period)
    {
      setPeriod(period.begin(), period.end());
    }

    void setMarker(const QString& marker)
    {
      m_marker    = marker;
    }

    void setPath(const QString& path)
    {
      m_path    = path;
    }

    void setFileName(const QString& file_name)
    {
      m_file_name = file_name;
    }

    void setData(const QString& marker, TaskPeriod period, const QString& path, const QString& file_name)
    {
      setData(marker, period.begin(), period.end(), path, file_name);
    }

    void setData(const QString& marker, QDateTime begin, QDateTime end, const QString& path, const QString& file_name)
    {
      setPeriod(begin, end);

      m_marker    = marker;
      m_path      = path;
      m_file_name = file_name;
    }

  private:

    qint64      m_id;
    QString     m_marker;
    QDateTime   m_begin;
    QDateTime   m_end;
    QString     m_path;
    QString     m_file_name;
    TaskPeriod  m_period;

  };

}
#endif // ZN_GLOBAL

