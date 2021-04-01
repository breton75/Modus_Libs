#ifndef RADUGA_DEFS_H
#define RADUGA_DEFS_H

#include <QMap>

#include "../../../../../Modus/global/signal/sv_signal.h"
#include "../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include "../../../../../Modus/global/global_defs.h"
//#include "../../../../../Modus/global/device/device_defs.h"

#define TYPE_1  1
#define TYPE_2  2
#define TYPE_3  3
#define TYPE_5  5
#define TYPE_9  9
#define TYPE_53 53

#define P_RADUGA_BYTE   "byte"
#define P_RADUGA_OFFSET "offset"
#define P_RADUGA_LEN    "len"


namespace raduga {

  struct DATA
  {
    DATA():
      data(nullptr),
      bufsize(0)
    {  }

    DATA(quint16 size):
      data(nullptr),
      bufsize(size)
    {
      data = (quint8*)malloc(size);
    }

    ~DATA()
    {
      if(data)
        free(data);
    }

    bool resize(quint16 size)
    {
      if(data)
        free(data);

      data = nullptr;

      bufsize = size;
      data = (quint8*)malloc(size);

      return bool(data);
    }

    quint8* data = nullptr;
    quint8  type;
    quint8  len;
    quint16 crc;

    quint16 bufsize;

  };

  struct SignalParams
  {
    quint16 byte    = 0;
    quint8  offset  = 0;
    quint8  len     = 0;

    static SignalParams fromJson(const QString& json_string) //throw (SvException)
    {
      QJsonParseError err;
      QJsonDocument jd = QJsonDocument::fromJson(json_string.toUtf8(), &err);

      if(err.error != QJsonParseError::NoError)
        throw SvException(err.errorString());

      try {

        return fromJsonObject(jd.object());

      }
      catch(SvException& e) {
        throw e;
      }
    }

    static SignalParams fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      SignalParams p;
      QString P;

      P = P_RADUGA_BYTE;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.byte = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер байта должен быть задан двухбайтовым целым числом в десятичном формате"));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      P = P_RADUGA_OFFSET;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.offset = h.toUShort(&ok, 0);

        if(!ok || p.offset > 7)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Сдвиг должен быть задан однобайтовым целым числом в десятичном формате в диапазоне [0..7]"));
      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));


      P = P_RADUGA_LEN;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.len = h.toUShort(&ok, 0);

        if(!ok || p.len > 7)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Кол-во бит должено быть задано однобайтовым целым числом в десятичном формате в диапазоне [0..7]"));
      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      return p;

    }

    QString toString(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const
    {
      QJsonDocument jd;
      jd.setObject(toJsonObject());

      return QString(jd.toJson(format));
    }

    QJsonObject toJsonObject() const
    {
      QJsonObject j;

      j.insert(P_RADUGA_BYTE,   QJsonValue(byte).toString());
      j.insert(P_RADUGA_OFFSET, QJsonValue(offset).toString());
      j.insert(P_RADUGA_LEN,    QJsonValue(len).toString());

      return j;

    }
  };

  class SvAbstractSignalCollection: public QObject
  {
    Q_OBJECT

  public:
    SvAbstractSignalCollection()
    {  }

    virtual ~SvAbstractSignalCollection()
    {  }

    virtual void addSignal(modus::SvSignal* signal) = 0;

    virtual void updateSignals(const raduga::DATA* data = nullptr) = 0;

  };
}

#endif // RADUGA_DEFS_H




