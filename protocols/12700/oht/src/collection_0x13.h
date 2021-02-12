#ifndef TYPE0X13_H
#define TYPE0X13_H

#include <QObject>
#include <QMap>
#include <QBitArray>

#include "oht_defs.h"

#define ROUTE_DATA_LENGTH 6

namespace oht {

  struct SignalParams_0x13
  {
    quint16 route = 0;
    quint8  byte  = 0;
    quint8  bit   = 0;
    quint8  len   = 0;

    static SignalParams_0x13 fromJson(const QString& json_string) //throw (SvException)
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

    static SignalParams_0x13 fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      SignalParams_0x13 p;
      QString P;

      // route (направление пожаротушения)
      P = P_ROUTE;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер направления должен быть задан целым числом"));

        p.route = quint16(object.value(P).toInt());

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      // byte
      P = P_BYTE;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер байта в пакете должен быть задан целым числом"));

        p.byte = quint16(object.value(P).toInt());

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      // bit
      P = P_BYTE;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер бита должен быть задан целым числом"));

        p.bit = quint16(object.value(P).toInt());

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      // len
      P = P_LEN;
      if(object.contains(P)) {

        if(object.value(P).toInt(-1) < 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Количество бит должно быть задано целым числом"));

        p.len = quint16(object.value(P).toInt());

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

      j.insert(P_ROUTE,  QJsonValue(route).toString());
      j.insert(P_NUMBER, QJsonValue(number).toString());

      return j;

    }

  };

  struct Signal0x13 {

    Signal0x13()
    { }

    Signal0x13(modus::SvSignal* signal, SignalParams_0x13 params):
      signal(signal), params(params)
    {  }

    modus::SvSignal* signal;
    SignalParams_0x13 params;

  };

  class Type0x13 : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit Type0x13();

    void addSignal(modus::SvSignal* signal); // throw (SvException);

    void updateSignals(const oht::DATA* data = nullptr);

  private:
//    QMap<quint32, modus::SvSignal*> m_signals;
    QMap<quint32, oht::Signal0x13> m_signals;

  };
}


#endif // TYPE0X13_H
