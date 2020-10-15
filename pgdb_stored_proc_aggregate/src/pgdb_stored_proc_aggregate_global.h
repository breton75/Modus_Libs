﻿#ifndef PGDB_STORED_PROC_GLOBAL_H
#define PGDB_STORED_PROC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PGDB_STORED_PROC_AGGREGATE_LIBRARY)
#  define PGDB_STORED_PROC_AGGREGATE_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define PGDB_STORED_PROC_AGGREGATE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PGDB_STORED_PROC_GLOBAL_H
