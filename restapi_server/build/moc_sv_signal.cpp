/****************************************************************************
** Meta object code from reading C++ file 'sv_signal.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Modus/global/signal/sv_signal.h"
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
    QByteArrayData data[14];
    char stringdata0[112];
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
QT_MOC_LITERAL(5, 42, 7), // "message"
QT_MOC_LITERAL(6, 50, 3), // "msg"
QT_MOC_LITERAL(7, 54, 5), // "level"
QT_MOC_LITERAL(8, 60, 4), // "type"
QT_MOC_LITERAL(9, 65, 8), // "setValue"
QT_MOC_LITERAL(10, 74, 5), // "value"
QT_MOC_LITERAL(11, 80, 17), // "setDeviceAliveAge"
QT_MOC_LITERAL(12, 98, 9), // "alive_age"
QT_MOC_LITERAL(13, 108, 3) // "log"

    },
    "modus::SvSignal\0changed\0\0SvSignal*\0"
    "signal\0message\0msg\0level\0type\0setValue\0"
    "value\0setDeviceAliveAge\0alive_age\0log"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_modus__SvSignal[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    3,   62,    2, 0x06 /* Public */,
       5,    2,   69,    2, 0x26 /* Public | MethodCloned */,
       5,    1,   74,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   77,    2, 0x0a /* Public */,
      11,    1,   80,    2, 0x0a /* Public */,
      13,    3,   83,    2, 0x0a /* Public */,
      13,    2,   90,    2, 0x2a /* Public | MethodCloned */,
      13,    1,   95,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    6,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,   10,
    QMetaType::Void, QMetaType::ULongLong,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    6,    7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void modus::SvSignal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvSignal *_t = static_cast<SvSignal *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changed((*reinterpret_cast< SvSignal*(*)>(_a[1]))); break;
        case 1: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->setValue((*reinterpret_cast< const QVariant(*)>(_a[1]))); break;
        case 5: _t->setDeviceAliveAge((*reinterpret_cast< const quint64(*)>(_a[1]))); break;
        case 6: _t->log((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: _t->log((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
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
            typedef void (SvSignal::*_t)(const QString , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvSignal::message)) {
                *result = 1;
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
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
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
void modus::SvSignal::message(const QString _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
