/**********************************************************************
 *  константы и структуры для работы с последовательным портом.
 *  форма для редактирования находится в файлах sv_serial_editor.*
 *  ВАЖНО! решено разнести парметры и редактор по разным фалам,
 *  чтобы не было проблемы при работе с приложениями без GUI
 *
 *  автор Свиридов С.А. Авиационные и Морская Электроника
 * *********************************************************************/

#ifndef LOGAPI_PARAMS
#define LOGAPI_PARAMS

#include <QtGlobal>
#include <QtCore/QCommandLineParser>
#include <QUdpSocket>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../../svlib/sv_exception.h"
#include "../../../../Modus/global/global_defs.h"

// имена параметров
#define P_PORT                  "port"
#define P_INDEX_FILE_NAME       "index_file_name"
#define P_PATH                  "path"

#define DEFAULT_PORT            80
#define DEFAULT_INDEX_FILE_NAME "index.html"
#define DEFAULT_PATH            "html/log"

/** структура для хранения параметров **/
namespace logapi {

  struct Params {

    quint16 port       = 80;
    QString index_file = DEFAULT_INDEX_FILE_NAME;
    QString html_path  = DEFAULT_PATH;

    static Params fromJsonString(const QString& json) throw (SvException)
    {
      QJsonParseError err;
      QJsonDocument jd = QJsonDocument::fromJson(json.toUtf8(), &err);

      if(err.error != QJsonParseError::NoError)
        throw SvException(err.errorString());

      try {
        return fromJsonObject(jd.object());
      }
      catch(SvException e) {
        throw e;
      }
    }

    static Params fromJsonObject(const QJsonObject &object) throw (SvException)
    {
      Params p;
      QString P;

      /* port */
      P = P_PORT;
      if(object.contains(P)) {

        p.port = object.value(P).toInt(0);

        if(p.port == 0)
          throw SvException(QString(IMPERMISSIBLE_VALUE)
                             .arg(P).arg(object.value(P).toVariant().toString())
                             .arg("Допускаются только числовые значения в диапазоне 1 - 65535"));

      }
      else p.port = 80;

      /* index file name */
      P = P_INDEX_FILE_NAME;
      p.index_file = object.contains(P) ? object.value(P).toString() : DEFAULT_INDEX_FILE_NAME;

      /* html_path */
      P = P_PATH;
      p.html_path = object.contains(P) ? object.value(P).toString() : DEFAULT_PATH;


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

      j.insert(P_PORT,            QJsonValue(static_cast<int>(port)).toInt());
      j.insert(P_INDEX_FILE_NAME, QJsonValue(index_file).toString());
      j.insert(P_PATH,            QJsonValue(html_path).toString());

      return j;

    }
  };
}


#endif // LOGAPI_PARAMS

