#ifndef TYPE0X03_H
#define TYPE0X03_H

#include <QObject>
#include <QMap>

#include "opa_defs.h"

namespace opa {

  struct SignalParams_0x03
  {
    quint16 room  = 0;
    quint8  level = 0;

    static SignalParams_0x03 fromJson(const QString& json_string) throw (SvException)
    {
      QJsonParseError err;
      QJsonDocument jd = QJsonDocument::fromJson(json_string.toUtf8(), &err);

      if(err.error != QJsonParseError::NoError)
        throw SvException(err.errorString());

      try {

        return fromJsonObject(jd.object());

      }
      catch(SvException e) {
        throw e;
      }
    }

    static SignalParams_0x03 fromJsonObject(const QJsonObject &object) throw (SvException)
    {
      SignalParams_0x03 p;
      QString P;

      P = P_OPA_ROOM;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.room = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Номер помещения должен быть задан двухбайтовым целым числом в шестнадцатиречном, "
                                 "восьмеричном или десятичном формате в кавычках: \"0xFFFF\" | \"0177777\" | \"65535\""));

      }
      else
        throw SvException(QString(E_NO_PARAM).arg(P));

      P = P_OPA_LEVEL;
      if(object.contains(P)) {

        QByteArray h = object.value(P).toString().toUtf8();

        bool ok = false;
        p.level = h.toUShort(&ok, 0);

        if(!ok)
          throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                            .arg(P)
                            .arg(object.value(P).toVariant().toString())
                            .arg("Уровень пожарной опасности должен быть задан однобайтовым целым числом в шестнадцатиречном, "
                                 "восьмеричном или десятичном формате в кавычках: \"0xFF\" | \"0377\" | \"255\""));

      }
      else
        throw SvException(QString(E_NO_PARAM).arg(P));


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

      j.insert(P_OPA_ROOM,  QJsonValue(room).toString());
      j.insert(P_OPA_LEVEL, QJsonValue(level).toString());

      return j;

    }
  };

  class Type0x03 : public SvAbstractSignalCollection
  {
    Q_OBJECT
  public:
    explicit Type0x03();

    void addSignal(SvSignal* signal) throw (SvException);

    void updateSignals(const opa::DATA* data = nullptr);

  private:
    QMap<quint32, SvSignal*> m_signals;

  };
}


#endif // TYPE0X02_H
