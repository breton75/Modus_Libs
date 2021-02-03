#ifndef IFC_RS_GLOBAL_H
#define IFC_RS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IFC_RS_LIBRARY)
#  define IFC_RS_EXPORT Q_DECL_EXPORT
#else
#  define IFC_RS_EXPORT Q_DECL_IMPORT
#endif

#endif // IFC_RS_GLOBAL_H
