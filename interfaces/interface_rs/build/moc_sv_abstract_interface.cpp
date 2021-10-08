/****************************************************************************
** Meta object code from reading C++ file 'sv_abstract_interface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Modus/global/device/interface/sv_abstract_interface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_abstract_interface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_modus__SvAbstractInterface_t {
    QByteArrayData data[14];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_modus__SvAbstractInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_modus__SvAbstractInterface_t qt_meta_stringdata_modus__SvAbstractInterface = {
    {
QT_MOC_LITERAL(0, 0, 26), // "modus::SvAbstractInterface"
QT_MOC_LITERAL(1, 27, 7), // "message"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 3), // "msg"
QT_MOC_LITERAL(4, 40, 5), // "level"
QT_MOC_LITERAL(5, 46, 4), // "type"
QT_MOC_LITERAL(6, 51, 8), // "finished"
QT_MOC_LITERAL(7, 60, 5), // "error"
QT_MOC_LITERAL(8, 66, 5), // "start"
QT_MOC_LITERAL(9, 72, 4), // "read"
QT_MOC_LITERAL(10, 77, 5), // "write"
QT_MOC_LITERAL(11, 83, 12), // "modus::BUFF*"
QT_MOC_LITERAL(12, 96, 6), // "buffer"
QT_MOC_LITERAL(13, 103, 4) // "stop"

    },
    "modus::SvAbstractInterface\0message\0\0"
    "msg\0level\0type\0finished\0error\0start\0"
    "read\0write\0modus::BUFF*\0buffer\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_modus__SvAbstractInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   59,    2, 0x06 /* Public */,
       1,    2,   66,    2, 0x26 /* Public | MethodCloned */,
       1,    1,   71,    2, 0x26 /* Public | MethodCloned */,
       6,    0,   74,    2, 0x06 /* Public */,
       7,    0,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   76,    2, 0x0a /* Public */,
       9,    0,   77,    2, 0x0a /* Public */,
      10,    1,   78,    2, 0x0a /* Public */,
      13,    0,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,

       0        // eod
};

void modus::SvAbstractInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvAbstractInterface *_t = static_cast<SvAbstractInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->finished(); break;
        case 4: _t->error(); break;
        case 5: { bool _r = _t->start();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: _t->read(); break;
        case 7: _t->write((*reinterpret_cast< modus::BUFF*(*)>(_a[1]))); break;
        case 8: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SvAbstractInterface::*_t)(const QString , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvAbstractInterface::message)) {
                *result = 0;
            }
        }
        {
            typedef void (SvAbstractInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvAbstractInterface::finished)) {
                *result = 3;
            }
        }
        {
            typedef void (SvAbstractInterface::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvAbstractInterface::error)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject modus::SvAbstractInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_modus__SvAbstractInterface.data,
      qt_meta_data_modus__SvAbstractInterface,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *modus::SvAbstractInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *modus::SvAbstractInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_modus__SvAbstractInterface.stringdata0))
        return static_cast<void*>(const_cast< SvAbstractInterface*>(this));
    return QObject::qt_metacast(_clname);
}

int modus::SvAbstractInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void modus::SvAbstractInterface::message(const QString _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 3
void modus::SvAbstractInterface::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void modus::SvAbstractInterface::error()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
