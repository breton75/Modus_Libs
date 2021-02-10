/****************************************************************************
** Meta object code from reading C++ file 'sv_ises.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/sv_ises.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_ises.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ises__SvISES_t {
    QByteArrayData data[6];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ises__SvISES_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ises__SvISES_t qt_meta_stringdata_ises__SvISES = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ises::SvISES"
QT_MOC_LITERAL(1, 13, 12), // "deleteThread"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 6), // "logerr"
QT_MOC_LITERAL(4, 34, 1), // "e"
QT_MOC_LITERAL(5, 36, 12) // "logreconnect"

    },
    "ises::SvISES\0deleteThread\0\0logerr\0e\0"
    "logreconnect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ises__SvISES[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       5,    0,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,

       0        // eod
};

void ises::SvISES::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvISES *_t = static_cast<SvISES *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deleteThread(); break;
        case 1: _t->logerr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->logreconnect(); break;
        default: ;
        }
    }
}

const QMetaObject ises::SvISES::staticMetaObject = {
    { &as::SvAbstractStorage::staticMetaObject, qt_meta_stringdata_ises__SvISES.data,
      qt_meta_data_ises__SvISES,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ises::SvISES::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ises::SvISES::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ises__SvISES.stringdata0))
        return static_cast<void*>(const_cast< SvISES*>(this));
    return as::SvAbstractStorage::qt_metacast(_clname);
}

int ises::SvISES::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = as::SvAbstractStorage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
struct qt_meta_stringdata_ises__SvISESThread_t {
    QByteArrayData data[5];
    char stringdata0[38];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ises__SvISESThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ises__SvISESThread_t qt_meta_stringdata_ises__SvISESThread = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ises::SvISESThread"
QT_MOC_LITERAL(1, 19, 5), // "error"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 1), // "e"
QT_MOC_LITERAL(4, 28, 9) // "connected"

    },
    "ises::SvISESThread\0error\0\0e\0connected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ises__SvISESThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    0,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void ises::SvISESThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvISESThread *_t = static_cast<SvISESThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->connected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SvISESThread::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvISESThread::error)) {
                *result = 0;
            }
        }
        {
            typedef void (SvISESThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvISESThread::connected)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ises::SvISESThread::staticMetaObject = {
    { &as::SvAbstractStorageThread::staticMetaObject, qt_meta_stringdata_ises__SvISESThread.data,
      qt_meta_data_ises__SvISESThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ises::SvISESThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ises::SvISESThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ises__SvISESThread.stringdata0))
        return static_cast<void*>(const_cast< SvISESThread*>(this));
    return as::SvAbstractStorageThread::qt_metacast(_clname);
}

int ises::SvISESThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = as::SvAbstractStorageThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ises::SvISESThread::error(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ises::SvISESThread::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
