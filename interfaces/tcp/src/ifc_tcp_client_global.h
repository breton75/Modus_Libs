#ifndef IFC_TCP_GLOBAL_H
#define IFC_TCP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IFC_TCP_LIBRARY)
#  define IFC_TCP_EXPORT Q_DECL_EXPORT
#else
#  define IFC_TCP_EXPORT Q_DECL_IMPORT
#endif

#endif // IFC_TCP_GLOBAL_H
