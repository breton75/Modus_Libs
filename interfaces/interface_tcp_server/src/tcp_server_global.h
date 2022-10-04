#ifndef TCP_SERVER_GLOBAL_H
#define TCP_SERVER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TCP_SERVER_LIBRARY)
#  define TCP_SERVER_EXPORT Q_DECL_EXPORT
#else
#  define TCP_SERVER_EXPORT Q_DECL_IMPORT
#endif

#endif // TCP_SERVER_GLOBAL_H
