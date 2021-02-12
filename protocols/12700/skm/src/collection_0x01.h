#ifndef TYPE0X01_H
#define TYPE0X01_H

#include <QObject>
#include <QMap>

#include "skm_defs.h"

namespace skm {

  struct SignalParams_0x01
  {
    quint8 vin    = 0;
    quint8 faktor = 0;

    static SignalParams_0x01 fromJson(const QString& json_string) //throw (SvException)
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

    static SignalParams_0x01 fromJsonObject(const QJsonObject &object) //throw (SvException)
    {
      SignalParams_0x01 p;
      QString P;

      // vin
      P = P_SKM_VIN;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.vin = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер ВИН камеры должен быть задан однобайтовым целым числом в шестнадцатиречном, "
                                 "восьмеричном или десятичном формате в кавычках: \"0xFF\" | \"0377\" | \"255\""));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      // faktor
      P = P_SKM_FAKTOR;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.faktor = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Фактор сработки должен быть задан однобайтовым целым числом в шестнадцатиречном, "
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

      j.insert(P_SKM_VIN,    QJsonValue(vin).toString());
      j.insert(P_SKM_FAKTOR, QJsonValue(faktor).toString());

      return j;

    }
  };

  class Type0x01 : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit Type0x01();

    void addSignal(modus::SvSignal* signal); // throw (SvException);

    void updateSignals(const skm::DATA* data = nullptr);

  private:
    QMap<quint32, modus::SvSignal*> m_signals;


  };
}


#endif // TYPE0X01_H
