#ifndef TYPE0X02_H
#define TYPE0X02_H

#include <QObject>
#include <QPair>
#include <QMap>

#include "../../../Modus/global/sv_abstract_data_type.h"

typedef QPair<quint16, quint8> SignalMarker;
typedef QMap<quint32, QString> SignalsMap;

// имена параметров
#define P_SENSOR  "sensor"
#define P_FAKTOR  "faktor"

#define E_IMPERMISSIBLE_VALUE "Недопустимое значение параметра %1: %2.\n%3"
#define E_NO_PARAM  "Для сигнала не задан обязательный параметр \"%1\""

struct SignalParams_0x02
{
  quint16 sensor = 0;
  quint8  faktor = 0;

  static SignalParams_0x02 fromJson(const QString& json_string) throw (SvException)
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

  static SignalParams_0x02 fromJsonObject(const QJsonObject &object) throw (SvException)
  {
    SignalParams_0x02 p;
    QString P;

    P = P_SENSOR;
    if(object.contains(P)) {

      QByteArray h = object.value(P).toString().toUtf8();

      bool ok = false;
      p.sensor = h.toUShort(&ok, 0);

      if(!ok)
        throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                          .arg(P)
                          .arg(object.value(P).toVariant().toString())
                          .arg("Номер датчика должен быть двухбайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFFFF | 0177777 | 65535]"));

    }
    else
      throw SvException(QString(E_NO_PARAM).arg(P));

    P = P_FAKTOR;
    if(object.contains(P)) {

      QByteArray h = object.value(P).toString().toUtf8();

      bool ok = false;
      p.faktor = h.toUShort(&ok, 0);

      if(!ok)
        throw SvException(QString(E_IMPERMISSIBLE_VALUE)
                          .arg(P)
                          .arg(object.value(P).toVariant().toString())
                          .arg("Фактор сработки должен быть однобайтовым целым числом в шестнадцатиречном, восьмеричном или десятичном формате: [0xFF | 01777 | 255]"));

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
    QString r;

    r = QString::number(sensor, 16);

//    if(r.length() < 4)
//      r.push_front(QString(4 - r.length(), QChar('0')));

    QString sensor_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);


    r = QString::number(faktor, 16);

//    if(r.length() < 2)
//      r.push_front(QString(2 - r.length(), QChar('0')));

    QString faktor_r = QString("0x%1").arg(r.length() % 2 ? "0" + r : r);


    j.insert(P_SENSOR, QJsonValue(sensor_r).toString());
    j.insert(P_FAKTOR, QJsonValue(faktor_r).toString());

    return j;

  }
};

class Type0x02 : public SvAbstractDataType
{
  Q_OBJECT
public:
  explicit Type0x02(QObject *parent = 0);

  void addSignal(const SvSignal* signal) throw (SvException);

  void updateSignals(const ad::DATA* data);

private:
  SignalsMap m_signals;

signals:

public slots:
};

#endif // TYPE0X02_H
