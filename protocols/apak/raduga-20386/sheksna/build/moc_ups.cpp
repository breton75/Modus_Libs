/****************************************************************************
** Meta object code from reading C++ file 'ups.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../APAK/protocols/ups/src/ups.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ups.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_apak__SvUPS_t {
    QByteArrayData data[11];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_apak__SvUPS_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_apak__SvUPS_t qt_meta_stringdata_apak__SvUPS = {
    {
QT_MOC_LITERAL(0, 0, 11), // "apak::SvUPS"
QT_MOC_LITERAL(1, 12, 5), // "start"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 13), // "signalUpdated"
QT_MOC_LITERAL(4, 33, 16), // "modus::SvSignal*"
QT_MOC_LITERAL(5, 50, 6), // "signal"
QT_MOC_LITERAL(6, 57, 13), // "signalChanged"
QT_MOC_LITERAL(7, 71, 14), // "queued_request"
QT_MOC_LITERAL(8, 86, 9), // "on_answer"
QT_MOC_LITERAL(9, 96, 12), // "modus::BUFF*"
QT_MOC_LITERAL(10, 109, 6) // "buffer"

    },
    "apak::SvUPS\0start\0\0signalUpdated\0"
    "modus::SvSignal*\0signal\0signalChanged\0"
    "queued_request\0on_answer\0modus::BUFF*\0"
    "buffer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_apak__SvUPS[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    1,   40,    2, 0x0a /* Public */,
       6,    1,   43,    2, 0x0a /* Public */,
       7,    0,   46,    2, 0x08 /* Private */,
       8,    1,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void apak::SvUPS::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvUPS *_t = static_cast<SvUPS *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start(); break;
        case 1: _t->signalUpdated((*reinterpret_cast< modus::SvSignal*(*)>(_a[1]))); break;
        case 2: _t->signalChanged((*reinterpret_cast< modus::SvSignal*(*)>(_a[1]))); break;
        case 3: _t->queued_request(); break;
        case 4: _t->on_answer((*reinterpret_cast< modus::BUFF*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< modus::SvSignal* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< modus::SvSignal* >(); break;
            }
            break;
        }
    }
}

const QMetaObject apak::SvUPS::staticMetaObject = {
    { &modus::SvAbstractProtocol::staticMetaObject, qt_meta_stringdata_apak__SvUPS.data,
      qt_meta_data_apak__SvUPS,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *apak::SvUPS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *apak::SvUPS::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_apak__SvUPS.stringdata0))
        return static_cast<void*>(const_cast< SvUPS*>(this));
    return modus::SvAbstractProtocol::qt_metacast(_clname);
}

int apak::SvUPS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = modus::SvAbstractProtocol::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
