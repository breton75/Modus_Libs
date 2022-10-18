﻿#ifndef TCP_CLIENT_GLOBAL_H
#define TCP_CLIENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TCP_CLIENT_LIBRARY)
#  define TCP_CLIENT_EXPORT Q_DECL_EXPORT
#else
#  define TCP_CLIENT_EXPORT Q_DECL_IMPORT
#endif

#endif // TCP_CLIENT_GLOBAL_H
