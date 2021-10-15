/****************************************************************************
** Meta object code from reading C++ file 'sv_signal.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../Modus/global/signal/sv_signal.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_signal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_modus__SvSignal_t {
    QByteArrayData data[15];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_modus__SvSignal_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_modus__SvSignal_t qt_meta_stringdata_modus__SvSignal = {
    {
QT_MOC_LITERAL(0, 0, 15), // "modus::SvSignal"
QT_MOC_LITERAL(1, 16, 7), // "changed"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 9), // "SvSignal*"
QT_MOC_LITERAL(4, 35, 6), // "signal"
QT_MOC_LITERAL(5, 42, 7), // "updated"
QT_MOC_LITERAL(6, 50, 7), // "timeout"
QT_MOC_LITERAL(7, 58, 7), // "message"
QT_MOC_LITERAL(8, 66, 3), // "msg"
QT_MOC_LITERAL(9, 70, 5), // "level"
QT_MOC_LITERAL(10, 76, 4), // "type"
QT_MOC_LITERAL(11, 81, 8), // "setValue"
QT_MOC_LITERAL(12, 90, 5), // "value"
QT_MOC_LITERAL(13, 96, 3), // "log"
QT_MOC_LITERAL(14, 100, 7) // "Timeout"

    },
    "modus::SvSignal\0changed\0\0SvSignal*\0"
    "signal\0updated\0timeout\0message\0msg\0"
    "level\0type\0setValue\0value\0log\0Timeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_modus__SvSignal[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    1,   72,    2, 0x06 /* Public */,
       6,    1,   75,    2, 0x06 /* Public */,
       7,    3,   78,    2, 0x06 /* Public */,
       7,    2,   85,    2, 0x26 /* Public | MethodCloned */,
       7,    1,   90,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   93,    2, 0x0a /* Public */,
      13,    3,   96,    2, 0x0a /* Public */,
      13,    2,  103,    2, 0x2a /* Public | MethodCloned */,
      13,    1,  108,    2, 0x2a /* Public | MethodCloned */,
      14,    0,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

void modus::SvSignal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvSignal *_t = static_cast<SvSignal *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changed((*reinterpret_cast< SvSignal*(*)>(_a[1]))); break;
        case 1: _t->updated((*reinterpret_cast< SvSignal*(*)>(_a[1]))); break;
        case 2: _t->timeout((*reinterpret_cast< SvSignal*(*)>(_a[1]))); break;
        case 3: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->setValue((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 7: _t->log((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->log((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->Timeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SvSignal* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SvSignal* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SvSignal* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SvSignal::*_t)(SvSignal * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvSignal::changed)) {
                *result = 0;
            }
        }
        {
            typedef void (SvSignal::*_t)(SvSignal * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvSignal::updated)) {
                *result = 1;
            }
        }
        {
            typedef void (SvSignal::*_t)(SvSignal * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvSignal::timeout)) {
                *result = 2;
            }
        }
        {
            typedef void (SvSignal::*_t)(const QString , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvSignal::message)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject modus::SvSignal::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_modus__SvSignal.data,
      qt_meta_data_modus__SvSignal,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *modus::SvSignal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *modus::SvSignal::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_modus__SvSignal.stringdata0))
        return static_cast<void*>(const_cast< SvSignal*>(this));
    return QObject::qt_metacast(_clname);
}

int modus::SvSignal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void modus::SvSignal::changed(SvSignal * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void modus::SvSignal::updated(SvSignal * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void modus::SvSignal::timeout(SvSignal * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void modus::SvSignal::message(const QString _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
