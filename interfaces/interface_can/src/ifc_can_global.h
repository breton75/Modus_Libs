#ifndef IFC_CAN_GLOBAL_H
#define IFC_CAN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IFC_CAN_LIBRARY)
#  define IFC_CAN_EXPORT Q_DECL_EXPORT
#else
#  define IFC_CAN_EXPORT Q_DECL_IMPORT
#endif

#endif // IFC_RS_GLOBAL_H
