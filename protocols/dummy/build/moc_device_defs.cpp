/****************************************************************************
** Meta object code from reading C++ file 'device_defs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Modus/global/device/device_defs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'device_defs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_modus__IOBuffer_t {
    QByteArrayData data[9];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_modus__IOBuffer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_modus__IOBuffer_t qt_meta_stringdata_modus__IOBuffer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "modus::IOBuffer"
QT_MOC_LITERAL(1, 16, 10), // "readyWrite"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12), // "modus::BUFF*"
QT_MOC_LITERAL(4, 41, 6), // "buffer"
QT_MOC_LITERAL(5, 48, 10), // "dataReaded"
QT_MOC_LITERAL(6, 59, 6), // "notice"
QT_MOC_LITERAL(7, 66, 3), // "say"
QT_MOC_LITERAL(8, 70, 2) // "ba"

    },
    "modus::IOBuffer\0readyWrite\0\0modus::BUFF*\0"
    "buffer\0dataReaded\0notice\0say\0ba"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_modus__IOBuffer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       6,    1,   40,    2, 0x06 /* Public */,
       7,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QByteArray,    8,

       0        // eod
};

void modus::IOBuffer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IOBuffer *_t = static_cast<IOBuffer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readyWrite((*reinterpret_cast< modus::BUFF*(*)>(_a[1]))); break;
        case 1: _t->dataReaded((*reinterpret_cast< modus::BUFF*(*)>(_a[1]))); break;
        case 2: _t->notice((*reinterpret_cast< modus::BUFF*(*)>(_a[1]))); break;
        case 3: _t->say((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (IOBuffer::*_t)(modus::BUFF * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IOBuffer::readyWrite)) {
                *result = 0;
            }
        }
        {
            typedef void (IOBuffer::*_t)(modus::BUFF * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IOBuffer::dataReaded)) {
                *result = 1;
            }
        }
        {
            typedef void (IOBuffer::*_t)(modus::BUFF * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IOBuffer::notice)) {
                *result = 2;
            }
        }
        {
            typedef void (IOBuffer::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&IOBuffer::say)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject modus::IOBuffer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_modus__IOBuffer.data,
      qt_meta_data_modus__IOBuffer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *modus::IOBuffer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *modus::IOBuffer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_modus__IOBuffer.stringdata0))
        return static_cast<void*>(const_cast< IOBuffer*>(this));
    return QObject::qt_metacast(_clname);
}

int modus::IOBuffer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void modus::IOBuffer::readyWrite(modus::BUFF * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void modus::IOBuffer::dataReaded(modus::BUFF * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void modus::IOBuffer::notice(modus::BUFF * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void modus::IOBuffer::say(const QByteArray & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
