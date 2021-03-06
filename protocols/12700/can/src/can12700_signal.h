﻿#ifndef CAN12700_SIGNAL
#define CAN12700_SIGNAL

#include <linux/can.h>

#include <QObject>
#include <QMap>

#include "can_defs.h"
#include "../../../../../Modus/global/signal/sv_abstract_signal_collection.h"

#define P_CANID   "canid"
#define P_OFFSET  "offset"
#define P_LEN     "len"

namespace can {

  struct SignalParams
  {
    canid_t canid  = 0;      // unsigned int 32
    quint8  offset = 0;
    quint8  len    = 0;

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

      // canid
      P = P_CANID;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) <= 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Параметр 'canid' должен быть задан целым положительным числом"));

        p.canid = object.value(P).toInt();

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(QString("%1/%2").arg(P_PROTOCOL).arg(P_PARAMS)).arg(P));

      // offset
      P = P_OFFSET;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Параметр 'offset' должен быть задан целым положительным числом"));

        p.offset = object.value(P).toInt();

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(QString("%1/%2").arg(P_PROTOCOL).arg(P_PARAMS)).arg(P));

      // len
      P = P_LEN;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) <= 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Параметр 'len' должен быть задан целым положительным числом > 0"));

        p.len = object.value(P).toInt();

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(QString("%1/%2").arg(P_PROTOCOL).arg(P_PARAMS)).arg(P));

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

      j.insert(P_CANID,  QJsonValue(int(canid)).toInt());
      j.insert(P_OFFSET, QJsonValue(offset).toInt());
      j.insert(P_LEN, QJsonValue(len).toInt());

      return j;

    }
  };

  struct CANSignal
  {

    CANSignal()
    { }

    CANSignal(modus::SvSignal* signal, SignalParams params):
      signal(signal), params(params)
    {  }

    modus::SvSignal* signal;
    SignalParams params;

  };

  class CANSignalCollection : public modus::SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit CANSignalCollection();

    void addSignal(modus::SvSignal* signal) override; // throw (SvException);

    void updateSignals(const modus::DATA* data = nullptr) override;

    void updateSignals(const can_frame &frame);

  private:
    QMap<quint64, can::CANSignal> m_signals;


  };
}


#endif // CAN12700_SIGNAL
