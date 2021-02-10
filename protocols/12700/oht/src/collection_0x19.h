#ifndef TYPE0X19_H
#define TYPE0X19_H

#include <QObject>
#include <QList>

#include "oht_defs.h"

namespace oht {

  struct SignalParams_0x19
  {
    quint8 byte = 0;
    quint8 bit  = 0;

    static SignalParams_0x19 fromJson(const QString& json_string) //throw (SvException)
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

    static SignalParams_0x19 fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      SignalParams_0x19 p;
      QString P;

      P = P_BYTE;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.byte = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер байта должен быть задан однобайтовым целым числом в шестнадцатеричном, "
                                 "восьмеричном или десятичном формате в кавычках: \"0xFF\" | \"0377\" | \"255\""));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      P = P_BIT;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.bit = h.toUShort(&ok, 0);

        if(!ok || p.bit > 7)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер бита должен быть задан однобайтовым целым числом в шестнадцатеричном, "
                                 "восьмеричном или десятичном формате в диапазоне [0..7] в кавычках: \"0x07\" | \"07\" | \"7\""));
      }
      else
        p.bit = 0; // throw SvException(QString(E_NO_PARAM).arg(P));


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

      j.insert(P_BYTE, QJsonValue(QString::number(byte)).toString());
      j.insert(P_BIT,  QJsonValue(QString::number(bit)).toString());

      return j;

    }
  };

  struct Signal0x19 {

    Signal0x19(modus::SvSignal* signal, SignalParams_0x19 params):
      signal(signal), params(params)
    {  }

    modus::SvSignal* signal;
    SignalParams_0x19 params;

  };

  class Type0x19 : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit Type0x19();

    void addSignal(modus::SvSignal* signal); //throw (SvException);

    void updateSignals(const oht::DATA* data = nullptr);

  private:
    QList<Signal0x19> m_signals;

  };
}


#endif // TYPE0X19_H
