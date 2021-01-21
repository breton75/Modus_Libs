/****************************************************************************
** Meta object code from reading C++ file 'pgdb_stored_proc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/pgdb_stored_proc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pgdb_stored_proc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_pgsp__pgStoredProcStorage_t {
    QByteArrayData data[9];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pgsp__pgStoredProcStorage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pgsp__pgStoredProcStorage_t qt_meta_stringdata_pgsp__pgStoredProcStorage = {
    {
QT_MOC_LITERAL(0, 0, 25), // "pgsp::pgStoredProcStorage"
QT_MOC_LITERAL(1, 26, 9), // "reconnect"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 21), // "start_reconnect_timer"
QT_MOC_LITERAL(4, 59, 20), // "stop_reconnect_timer"
QT_MOC_LITERAL(5, 80, 12), // "deleteThread"
QT_MOC_LITERAL(6, 93, 6), // "logerr"
QT_MOC_LITERAL(7, 100, 1), // "e"
QT_MOC_LITERAL(8, 102, 12) // "logreconnect"

    },
    "pgsp::pgStoredProcStorage\0reconnect\0"
    "\0start_reconnect_timer\0stop_reconnect_timer\0"
    "deleteThread\0logerr\0e\0logreconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pgsp__pgStoredProcStorage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    1,   48,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,

       0        // eod
};

void pgsp::pgStoredProcStorage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        pgStoredProcStorage *_t = static_cast<pgStoredProcStorage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->reconnect(); break;
        case 1: _t->start_reconnect_timer(); break;
        case 2: _t->stop_reconnect_timer(); break;
        case 3: _t->deleteThread(); break;
        case 4: _t->logerr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->logreconnect(); break;
        default: ;
        }
    }
}

const QMetaObject pgsp::pgStoredProcStorage::staticMetaObject = {
    { &modus::SvAbstractStorage::staticMetaObject, qt_meta_stringdata_pgsp__pgStoredProcStorage.data,
      qt_meta_data_pgsp__pgStoredProcStorage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *pgsp::pgStoredProcStorage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pgsp::pgStoredProcStorage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_pgsp__pgStoredProcStorage.stringdata0))
        return static_cast<void*>(this);
    return modus::SvAbstractStorage::qt_metacast(_clname);
}

int pgsp::pgStoredProcStorage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = modus::SvAbstractStorage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
