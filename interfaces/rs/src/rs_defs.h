﻿/**********************************************************************
 *  константы и структуры для работы с последовательным портом.
 *  форма для редактирования находится в файлах sv_serial_editor.*
 *  ВАЖНО! решено разнести парметры и редактор по разным фалам,
 *  чтобы не было проблемы при работе с приложениями без GUI
 *
 *  автор Свиридов С.А. Авиационные и Морская Электроника
 * *********************************************************************/

#ifndef OPA_IFC_SERIAL
#define OPA_IFC_SERIAL

#include <QtGlobal>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMap>
#include <QVariant>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../svlib/SvException/svexception.h"
#include "../../../../Modus/global/global_defs.h"

// имена параметров для RS
#define P_SERIAL_BAUDRATE "baudrate"
#define P_SERIAL_PORTNAME "portname"
#define P_SERIAL_DATABITS "databits"
#define P_SERIAL_PARITY   "parity"
#define P_SERIAL_STOPBITS "stopbits"
#define P_SERIAL_FLOWCTRL "flowcontrol"
#define P_SERIAL_FMT      "fmt"
#define P_DTR_CONTROL     "dtr_control"


const QList<int> Baudrates = {75, 115, 134, 150, 300, 600, 1200, 1800, 2400, 4800, 7200, 9600, 14400, 19200, 38400, 57600, 115200, 128000};

const QMap<QSerialPort::DataBits, QString>    DataBits     = {{QSerialPort::Data5, "5"},
                                                              {QSerialPort::Data6, "6"},
                                                              {QSerialPort::Data7, "7"},
                                                              {QSerialPort::Data8, "8"}};

const QMap<QSerialPort::Parity, QString>      Parities     = {{QSerialPort::NoParity, "Нет"},
                                                              {QSerialPort::EvenParity, "Чет"},
                                                              {QSerialPort::OddParity, "Нечет"},
                                                              {QSerialPort::SpaceParity, "Пробел"},
                                                              {QSerialPort::MarkParity, "Маркер"}};

const QMap<QSerialPort::StopBits, QString>    StopBits     = {{QSerialPort::OneStop, "1"},
                                                              {QSerialPort::OneAndHalfStop, "1,5"},
                                                              {QSerialPort::TwoStop, "2"}};

const QMap<QSerialPort::FlowControl, QString> FlowControls = {{QSerialPort::NoFlowControl, "Нет"},
                                                              {QSerialPort::HardwareControl, "Аппаратное"},
                                                              {QSerialPort::SoftwareControl, "Программное"}};

#define DEFAULT_BAUDRATE    19200
#define DEFAULT_DATABITS    8
#define DEFAULT_PARITY      0
#define DEFAULT_STOPBITS    1
#define DEFAULT_FLOWCONTROL 0

/** структура для хранения параметров последовательного порта **/
struct SerialParams {

  QString                   portname    =     "ttyS0";
  quint32                   baudrate    =     19200;
  QSerialPort::DataBits     databits    =     QSerialPort::Data8;
  QSerialPort::Parity       parity      =     QSerialPort::NoParity;
  QSerialPort::StopBits     stopbits    =     QSerialPort::OneStop;
  QSerialPort::FlowControl  flowcontrol =     QSerialPort::NoFlowControl;
  quint16                   fmt         =     modus::HEX;
  quint16                   grain_gap   =     DEFAULT_GRAIN_GAP;
  bool                      dtr_control =     true;

  bool isValid = true;

  static SerialParams fromJsonString(const QString& json_string) //throw (SvException)
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

  static SerialParams fromJsonObject(const QJsonObject &object) //throw (SvException)
  {
    SerialParams p;
    QString P;

    P = P_SERIAL_PORTNAME;
    if(object.contains(P)) {

      p.portname = object.value(P).toString();

      if(p.portname.isEmpty())
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Имя порта не может быть пустым"));
    }
    else
      throw SvException(QString(IMPERMISSIBLE_VALUE)
                         .arg(P).arg(object.value(P).toVariant().toString())
                         .arg("Должно быть задано имя порта"));

    /* baudrate */
    P = P_SERIAL_BAUDRATE;
    if(object.contains(P)) {

      if(object.value(P).toInt(-1) <= 0)
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Параметр baudrate должен быть задан целым положительным числом"));

      p.baudrate = object.value(P).toInt(DEFAULT_BAUDRATE);

    }
    else
      p.baudrate = DEFAULT_BAUDRATE;

    /* databits */
    P = P_SERIAL_DATABITS;
    if(object.contains(P)) {

      if(!DataBits.keys().contains(static_cast<QSerialPort::DataBits>(object.value(P).toInt(-1))))
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Параметр должен быть задан целым положительным числом в диапазоне [5, 6, 7, 8]"));

      p.databits = static_cast<QSerialPort::DataBits>(object.value(P).toInt(DEFAULT_DATABITS));
    }
    else
      p.databits = QSerialPort::DataBits(static_cast<QSerialPort::DataBits>(DEFAULT_DATABITS));

    /* parity */
    P = P_SERIAL_PARITY;
    if(object.contains(P)) {

      if(!Parities.keys().contains(static_cast<QSerialPort::Parity>(object.value(P).toInt(-1))))
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Параметр должен быть задан целым положительным числом. Допустимые значения: 0 - NoParity, 2 - Even, 3 - Odd, 4 - Space, 5 - Mark"));

      p.parity = static_cast<QSerialPort::Parity>(object.value(P).toInt(DEFAULT_PARITY));

    }
    else
      p.parity = static_cast<QSerialPort::Parity>(DEFAULT_PARITY);

    /* stopbits */
    P = P_SERIAL_STOPBITS;
    if(object.contains(P)) {

      if(!StopBits.keys().contains(static_cast<QSerialPort::StopBits>(object.value(P).toInt(-1))))
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Параметр должен быть задан целым положительным числом. Допустимые значения: 1 - One, 2 - Two, 3 - OneAndHalf"));

      p.stopbits = static_cast<QSerialPort::StopBits>(object.value(P).toInt(DEFAULT_STOPBITS));

    }

    /* flowcontrol*/
    P = P_SERIAL_FLOWCTRL;
    if(object.contains(P)) {

      if(!FlowControls.keys().contains(static_cast<QSerialPort::FlowControl>(object.value(P).toInt(-1))))
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                           .arg(P).arg(object.value(P).toVariant().toString())
                           .arg("Параметр должен быть задан целым положительным числом. Допустимые значения: 0 - NoFlowControl, 1 - HardwareControl, 2 - SoftwareControl"));

      p.flowcontrol = static_cast<QSerialPort::FlowControl>(object.value(P).toInt(DEFAULT_FLOWCONTROL));

    }
    else
      p.flowcontrol = static_cast<QSerialPort::FlowControl>(DEFAULT_FLOWCONTROL);

    /* log fmt */
    P = P_SERIAL_FMT;
    if(object.contains(P)) {

      if(!object.value(P).isString())
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                          .arg(P).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
                          .arg(QString("Формат вывода данных должен быть задан строковым значением [\"hex\"|\"ascii\"|\"datalen\"]")));

      QString fmt = object.value(P).toString("hex").toLower();

      if(!modus::LogFormats.contains(fmt))
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                          .arg(P).arg(QString(QJsonDocument(object).toJson(QJsonDocument::Compact)))
                          .arg(QString("Не поддерживаемый формат вывода данных. Допустимые значения: [\"hex\"|\"ascii\"|\"datalen\"]")));

      p.fmt = modus::LogFormats.value(fmt);

    }
    else
      p.fmt = modus::HEX;

    /* grain gap*/
    P = P_GRAIN_GAP;
    if(object.contains(P)) {

      if(object.value(P).toInt(-1) < 1)
        throw SvException(QString(IMPERMISSIBLE_VALUE)
                          .arg(P)
                          .arg(object.value(P).toVariant().toString())
                          .arg("Интервал ожидания частей пакета не может быть меньше 1 мсек."));

      p.grain_gap = object.value(P).toInt(DEFAULT_GRAIN_GAP);

    }
    else
      p.grain_gap = quint16(DEFAULT_GRAIN_GAP);

    /* DTR coontrol */
    P = P_DTR_CONTROL;
    if(object.contains(P))
      p.dtr_control = object.value(P).toBool(true);

    else
      p.dtr_control = true;

    return p;

  }

  QString toJsonString(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const
  {
    QJsonDocument jd;
    jd.setObject(toJsonObject());

    return QString(jd.toJson(format));
  }

  QJsonObject toJsonObject() const
  {
    QJsonObject j;

    j.insert(P_SERIAL_PORTNAME, QJsonValue(portname).toString());
    j.insert(P_SERIAL_BAUDRATE, QJsonValue(static_cast<int>(baudrate)).toInt());
    j.insert(P_SERIAL_DATABITS, QJsonValue(static_cast<int>(databits)).toInt());
    j.insert(P_SERIAL_FLOWCTRL, QJsonValue(static_cast<int>(flowcontrol)).toInt());
    j.insert(P_SERIAL_PARITY,   QJsonValue(static_cast<int>(parity)).toInt());
    j.insert(P_SERIAL_STOPBITS, QJsonValue(static_cast<int>(stopbits)).toInt());

    return j;

  }
};


#endif // OPA_IFC_SERIAL

