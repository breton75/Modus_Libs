/****************************************************************************
** Meta object code from reading C++ file 'gamma_security_imitator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../imitator/gamma_security_imitator/src/gamma_security_imitator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamma_security_imitator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_apak__SvGammaSecurityImitator_t {
    QByteArrayData data[8];
    char stringdata0[94];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_apak__SvGammaSecurityImitator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_apak__SvGammaSecurityImitator_t qt_meta_stringdata_apak__SvGammaSecurityImitator = {
    {
QT_MOC_LITERAL(0, 0, 29), // "apak::SvGammaSecurityImitator"
QT_MOC_LITERAL(1, 30, 13), // "signalUpdated"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 16), // "modus::SvSignal*"
QT_MOC_LITERAL(4, 62, 6), // "signal"
QT_MOC_LITERAL(5, 69, 13), // "signalChanged"
QT_MOC_LITERAL(6, 83, 5), // "start"
QT_MOC_LITERAL(7, 89, 4) // "send"

    },
    "apak::SvGammaSecurityImitator\0"
    "signalUpdated\0\0modus::SvSignal*\0signal\0"
    "signalChanged\0start\0send"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_apak__SvGammaSecurityImitator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       5,    1,   37,    2, 0x0a /* Public */,
       6,    0,   40,    2, 0x0a /* Public */,
       7,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void apak::SvGammaSecurityImitator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvGammaSecurityImitator *_t = static_cast<SvGammaSecurityImitator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalUpdated((*reinterpret_cast< modus::SvSignal*(*)>(_a[1]))); break;
        case 1: _t->signalChanged((*reinterpret_cast< modus::SvSignal*(*)>(_a[1]))); break;
        case 2: _t->start(); break;
        case 3: _t->send(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< modus::SvSignal* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< modus::SvSignal* >(); break;
            }
            break;
        }
    }
}

const QMetaObject apak::SvGammaSecurityImitator::staticMetaObject = {
    { &modus::SvAbstractProtocol::staticMetaObject, qt_meta_stringdata_apak__SvGammaSecurityImitator.data,
      qt_meta_data_apak__SvGammaSecurityImitator,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *apak::SvGammaSecurityImitator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *apak::SvGammaSecurityImitator::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_apak__SvGammaSecurityImitator.stringdata0))
        return static_cast<void*>(const_cast< SvGammaSecurityImitator*>(this));
    return modus::SvAbstractProtocol::qt_metacast(_clname);
}

int apak::SvGammaSecurityImitator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = modus::SvAbstractProtocol::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
