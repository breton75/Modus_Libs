/****************************************************************************
** Meta object code from reading C++ file 'sv_abstract_interact.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Modus/global/interact/sv_abstract_interact.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_abstract_interact.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_modus__SvAbstractInteract_t {
    QByteArrayData data[7];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_modus__SvAbstractInteract_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_modus__SvAbstractInteract_t qt_meta_stringdata_modus__SvAbstractInteract = {
    {
QT_MOC_LITERAL(0, 0, 25), // "modus::SvAbstractInteract"
QT_MOC_LITERAL(1, 26, 7), // "message"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 3), // "msg"
QT_MOC_LITERAL(4, 39, 5), // "level"
QT_MOC_LITERAL(5, 45, 4), // "type"
QT_MOC_LITERAL(6, 50, 4) // "stop"

    },
    "modus::SvAbstractInteract\0message\0\0"
    "msg\0level\0type\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_modus__SvAbstractInteract[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x06 /* Public */,
       1,    2,   41,    2, 0x26 /* Public | MethodCloned */,
       1,    1,   46,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void modus::SvAbstractInteract::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvAbstractInteract *_t = static_cast<SvAbstractInteract *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SvAbstractInteract::*_t)(const QString , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvAbstractInteract::message)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject modus::SvAbstractInteract::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_modus__SvAbstractInteract.data,
      qt_meta_data_modus__SvAbstractInteract,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *modus::SvAbstractInteract::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *modus::SvAbstractInteract::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_modus__SvAbstractInteract.stringdata0))
        return static_cast<void*>(const_cast< SvAbstractInteract*>(this));
    return QThread::qt_metacast(_clname);
}

int modus::SvAbstractInteract::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void modus::SvAbstractInteract::message(const QString _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
