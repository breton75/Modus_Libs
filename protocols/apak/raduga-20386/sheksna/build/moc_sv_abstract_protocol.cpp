/****************************************************************************
** Meta object code from reading C++ file 'sv_abstract_protocol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../Modus/global/device/protocol/sv_abstract_protocol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_abstract_protocol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_modus__SvAbstractProtocol_t {
    QByteArrayData data[14];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_modus__SvAbstractProtocol_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_modus__SvAbstractProtocol_t qt_meta_stringdata_modus__SvAbstractProtocol = {
    {
QT_MOC_LITERAL(0, 0, 25), // "modus::SvAbstractProtocol"
QT_MOC_LITERAL(1, 26, 7), // "message"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 3), // "msg"
QT_MOC_LITERAL(4, 39, 5), // "level"
QT_MOC_LITERAL(5, 45, 4), // "type"
QT_MOC_LITERAL(6, 50, 8), // "finished"
QT_MOC_LITERAL(7, 59, 13), // "signalUpdated"
QT_MOC_LITERAL(8, 73, 16), // "modus::SvSignal*"
QT_MOC_LITERAL(9, 90, 6), // "signal"
QT_MOC_LITERAL(10, 97, 13), // "signalChanged"
QT_MOC_LITERAL(11, 111, 5), // "start"
QT_MOC_LITERAL(12, 117, 4), // "stop"
QT_MOC_LITERAL(13, 122, 14) // "checkupSignals"

    },
    "modus::SvAbstractProtocol\0message\0\0"
    "msg\0level\0type\0finished\0signalUpdated\0"
    "modus::SvSignal*\0signal\0signalChanged\0"
    "start\0stop\0checkupSignals"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_modus__SvAbstractProtocol[] = {

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
       1,    3,   59,    2, 0x06 /* Public */,
       1,    2,   66,    2, 0x26 /* Public | MethodCloned */,
       1,    1,   71,    2, 0x26 /* Public | MethodCloned */,
       6,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   75,    2, 0x0a /* Public */,
      10,    1,   78,    2, 0x0a /* Public */,
      11,    0,   81,    2, 0x0a /* Public */,
      12,    0,   82,    2, 0x0a /* Public */,
      13,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void modus::SvAbstractProtocol::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvAbstractProtocol *_t = static_cast<SvAbstractProtocol *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->finished(); break;
        case 4: _t->signalUpdated((*reinterpret_cast< modus::SvSignal*(*)>(_a[1]))); break;
        case 5: _t->signalChanged((*reinterpret_cast< modus::SvSignal*(*)>(_a[1]))); break;
        case 6: _t->start(); break;
        case 7: _t->stop(); break;
        case 8: _t->checkupSignals(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< modus::SvSignal* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< modus::SvSignal* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SvAbstractProtocol::*_t)(const QString , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvAbstractProtocol::message)) {
                *result = 0;
            }
        }
        {
            typedef void (SvAbstractProtocol::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvAbstractProtocol::finished)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject modus::SvAbstractProtocol::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_modus__SvAbstractProtocol.data,
      qt_meta_data_modus__SvAbstractProtocol,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *modus::SvAbstractProtocol::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *modus::SvAbstractProtocol::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_modus__SvAbstractProtocol.stringdata0))
        return static_cast<void*>(const_cast< SvAbstractProtocol*>(this));
    return QObject::qt_metacast(_clname);
}

int modus::SvAbstractProtocol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void modus::SvAbstractProtocol::message(const QString _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 3
void modus::SvAbstractProtocol::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
