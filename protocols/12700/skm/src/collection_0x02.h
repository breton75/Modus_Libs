#ifndef TYPE0X02_H
#define TYPE0X02_H

#include <QObject>
#include <QMap>
#include <QMultiMap>

#include "skm_defs.h"

namespace skm {

  struct SignalParams_0x02
  {
    quint8 byte = 0;
    quint8 bit = 0;

    static SignalParams_0x02 fromJson(const QString& json_string) //throw (SvException)
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

    static SignalParams_0x02 fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      SignalParams_0x02 p;
      QString P;

      P = P_SKM_BIT;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.byte = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер байта должен быть задан однобайтовым целым числом в шестнадцатиричном, "
                                 "восьмеричном или десятичном формате в кавычках: \"0xFF\" | \"0377\" | \"255\""));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      P = P_SKM_BIT;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.bit = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер бита должен быть задан однобайтовым целым числом в шестнадцатиричном, "
                                 "восьмеричном или десятичном формате в кавычках: \"0xFF\" | \"0377\" | \"255\""));

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

      j.insert(P_SKM_BYTE, QJsonValue(byte).toString());
      j.insert(P_SKM_BIT,  QJsonValue(bit).toString());

      return j;

    }
  };

  class Type0x02 : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit Type0x02();

    void addSignal(modus::SvSignal* signal); // throw (SvException);

    void updateSignals(const skm::DATA* data = nullptr);

  private:
    QMultiMap<quint32, modus::SvSignal*> m_signals;

  };
}


#endif // TYPE0X02_H
