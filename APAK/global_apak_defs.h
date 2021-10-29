#ifndef GLOBAL_LIB_DEFS
#define GLOBAL_LIB_DEFS

#include <QMap>

#define TYPE_DATA "data"
#define TYPE_STAT "stat"

namespace apak {

  enum LogFormat {
    HEX,
    ASCII,
    DATALEN
  };

  const QMap<QString, LogFormat> LogFormats = {{"hex", HEX}, {"ascii", ASCII}, {"datalen", DATALEN}};

}

#endif // GLOBAL_LIB_DEFS

