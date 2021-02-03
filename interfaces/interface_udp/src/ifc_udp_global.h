#ifndef IFC_UDP_GLOBAL_H
#define IFC_UDP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IFC_UDP_LIBRARY)
#  define IFC_UDP_EXPORT Q_DECL_EXPORT
#else
#  define IFC_UDP_EXPORT Q_DECL_IMPORT
#endif

#endif // IFC_UDP_GLOBAL_H
