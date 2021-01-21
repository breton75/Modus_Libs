#ifndef INTERACT_GLOBAL_H
#define INTERACT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(INTERACT_LIBRARY)
#  define INTERACT_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define INTERACT_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // INTERACT_GLOBAL_H
