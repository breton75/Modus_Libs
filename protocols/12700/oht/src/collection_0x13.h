#ifndef TYPE0X13_H
#define TYPE0X13_H

#include <QObject>
#include <QMap>

#include "oht_defs.h"

namespace oht {

  struct SignalParams_0x13
  {
    quint16 room  = 0;
    quint8  level = 0;

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

      P = P_ROOM;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.room = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер помещения должен быть задан двухбайтовым целым числом в шестнадцатиречном, "
                                 "восьмеричном или десятичном формате в кавычках: \"0xFFFF\" | \"0177777\" | \"65535\""));

      }
      else
        throw SvException(QString(MISSING_PARAM).arg(P));

      P = P_LEVEL;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.level = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Уровень пожарной опасности должен быть задан однобайтовым целым числом в шестнадцатиречном, "
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

      j.insert(P_ROOM,  QJsonValue(room).toString());
      j.insert(P_LEVEL, QJsonValue(level).toString());

      return j;

    }
  };

  class Type0x13 : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit Type0x13();

    void addSignal(modus::SvSignal* signal); // throw (SvException);

    void updateSignals(const oht::DATA* data = nullptr);

  private:
    QMap<quint32, modus::SvSignal*> m_signals;

  };
}


#endif // TYPE0X13_H
