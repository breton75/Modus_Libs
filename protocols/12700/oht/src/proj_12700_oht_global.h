#ifndef PROJ_12700_OHT_GLOBAL_H
#define PROJ_12700_OHT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROJ_12700_OHT_LIBRARY)
#  define PROJ_12700_OHT_EXPORT Q_DECL_EXPORT
#else
#  define PROJ_12700_OHT_EXPORT Q_DECL_IMPORT
#endif

#endif // PROJ_12700_OHT_GLOBAL_H
