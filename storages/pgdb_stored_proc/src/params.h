#ifndef PGSP_PARAMS_H
#define PGSP_PARAMS_H

#include <QtGlobal>
#include <QtCore/QCommandLineParser>
#include <QUdpSocket>
#include <QHostAddress>

#include <QJsonDocument>
#include <QJsonObject>

#include "../../../svlib/sv_exception.h"
#include "../../../Modus/global/global_defs.h"

#define P_HOST        "host"
#define P_PORT        "port"
#define P_USER        "user"
#define P_PASS        "pass"
#define P_ROLE        "role"
#define P_DB          "db"
#define P_PROC_NAME   "proc_name"
#define PROC_CALL     "select %1('%2', '%3');"

namespace pgsp {

    struct Params
    {
      QString db              = "";
      QString host            = "localhost";
      quint16 port            = 5432;
      QString login           = "postgres";
      QString pass            = "postgres";
      QString role            = "postgres";
      QString proc_name       = "set_values";

      static Params fromJson(const QString& json_string) //throw (SvException)
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

      static Params fromJsonObject(const QJsonObject &object) //throw (SvException)
      {
        // проверяем наличие основных полей
        QStringList l = QStringList() << P_HOST << P_DB;
        for(QString v: l)
          if(object.value(v).isUndefined())
            throw SvException(QString("В разделе \"storages\" отсутствует или не задан параметр '%1'").arg(v));

        QString P;
        Params p;

        /* db */
        P = P_DB;
        if(object.contains(P)) {

          p.db = object.value(P).toString("");

          if(p.db.isEmpty())
            throw SvException(QString(IMPERMISSIBLE_VALUE)
                              .arg(P)
                              .arg(object.value(P).toVariant().toString())
                              .arg("Имя базы данных не может быть пустым и должно быть заключено в кавычки"));
        }
        else
          throw SvException(QString(MISSING_PARAM).arg("storages").arg(P));

        /* host */
        P = P_HOST;
        if(object.contains(P)) {

          p.host = object.value(P).toString("");

          if(p.host.isEmpty() || (p.host.toLower() != "localhost" && QHostAddress(p.host).toIPv4Address() == 0))
            throw SvException(QString(IMPERMISSIBLE_VALUE)
                              .arg(P)
                              .arg(object.value(P).toVariant().toString())
                              .arg("Допустимы \"localhost\" или ip адрес в формате \"192.16.2.100\""));
        }
        else
          throw SvException(QString(MISSING_PARAM).arg("storages").arg(P));

        /* port */
        P = P_PORT;
        if(object.contains(P))
        {
          if(object.value(P).toInt(-1) < 1)
            throw SvException(QString(IMPERMISSIBLE_VALUE)
                              .arg(P)
                              .arg(object.value(P).toVariant().toString())
                              .arg("Порт должен быть числом в диапазоне 1000 - 65535"));

          p.port = object.value(P).toInt(5432);

        }
        else p.port = 5432;

        /* user */
        P = P_USER;
        p.login = object.contains(P) ? object.value(P).toString("postgres") : "postgres";

        /* pass */
        P = P_PASS;
        p.pass = object.contains(P) ? object.value(P).toString("postgres") : "postgres";

        /* role */
        P = P_ROLE;
        p.role = object.contains(P) ? object.value(P).toString("postgres") : "postgres";

        /* proc name */
        P = P_PROC_NAME;
        p.proc_name = object.contains(P) ? object.value(P).toString("set_values") : "set_values";

        return p;

      }

      QString toJson(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const
      {
        QJsonDocument jd;
        jd.setObject(toJsonObject());

        return QString(jd.toJson(format));
      }

      QJsonObject toJsonObject() const
      {
        QJsonObject j;

        j.insert(P_DB,   QJsonValue(db).toString());
        j.insert(P_HOST, QJsonValue(host).toString());
        j.insert(P_PORT, QJsonValue(static_cast<int>(port)).toInt());
        j.insert(P_USER, QJsonValue(login).toString());
        j.insert(P_PASS, QJsonValue(pass).toString());
        j.insert(P_PROC_NAME, QJsonValue(proc_name).toString());
        return j;

      }
    };
  }

#endif // PGSP_PARAMS_H
