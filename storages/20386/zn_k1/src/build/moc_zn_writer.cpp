/****************************************************************************
** Meta object code from reading C++ file 'zn_writer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../zn_k1/zn_writer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'zn_writer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_zn1__ZNWriter_t {
    QByteArrayData data[6];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_zn1__ZNWriter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_zn1__ZNWriter_t qt_meta_stringdata_zn1__ZNWriter = {
    {
QT_MOC_LITERAL(0, 0, 13), // "zn1::ZNWriter"
QT_MOC_LITERAL(1, 14, 13), // "signalUpdated"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 16), // "modus::SvSignal*"
QT_MOC_LITERAL(4, 46, 6), // "signal"
QT_MOC_LITERAL(5, 53, 13) // "signalChanged"

    },
    "zn1::ZNWriter\0signalUpdated\0\0"
    "modus::SvSignal*\0signal\0signalChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_zn1__ZNWriter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       5,    1,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void zn1::ZNWriter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ZNWriter *_t = static_cast<ZNWriter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalUpdated((*reinterpret_cast< modus::SvSignal*(*)>(_a[1]))); break;
        case 1: _t->signalChanged((*reinterpret_cast< modus::SvSignal*(*)>(_a[1]))); break;
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

const QMetaObject zn1::ZNWriter::staticMetaObject = {
    { &modus::SvAbstractStorage::staticMetaObject, qt_meta_stringdata_zn1__ZNWriter.data,
      qt_meta_data_zn1__ZNWriter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *zn1::ZNWriter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *zn1::ZNWriter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_zn1__ZNWriter.stringdata0))
        return static_cast<void*>(const_cast< ZNWriter*>(this));
    return modus::SvAbstractStorage::qt_metacast(_clname);
}

int zn1::ZNWriter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = modus::SvAbstractStorage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
