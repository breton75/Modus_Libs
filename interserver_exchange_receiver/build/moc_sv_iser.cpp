/****************************************************************************
** Meta object code from reading C++ file 'sv_iser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/sv_iser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_iser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_iser__SvISER_t {
    QByteArrayData data[4];
    char stringdata0[38];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_iser__SvISER_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_iser__SvISER_t qt_meta_stringdata_iser__SvISER = {
    {
QT_MOC_LITERAL(0, 0, 12), // "iser::SvISER"
QT_MOC_LITERAL(1, 13, 10), // "stopThread"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12) // "deleteThread"

    },
    "iser::SvISER\0stopThread\0\0deleteThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_iser__SvISER[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void iser::SvISER::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvISER *_t = static_cast<SvISER *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stopThread(); break;
        case 1: _t->deleteThread(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SvISER::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvISER::stopThread)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject iser::SvISER::staticMetaObject = {
    { &ad::SvAbstractDevice::staticMetaObject, qt_meta_stringdata_iser__SvISER.data,
      qt_meta_data_iser__SvISER,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *iser::SvISER::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *iser::SvISER::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_iser__SvISER.stringdata0))
        return static_cast<void*>(const_cast< SvISER*>(this));
    return ad::SvAbstractDevice::qt_metacast(_clname);
}

int iser::SvISER::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ad::SvAbstractDevice::qt_metacall(_c, _id, _a);
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
void iser::SvISER::stopThread()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_iser__GenericThread_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_iser__GenericThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_iser__GenericThread_t qt_meta_stringdata_iser__GenericThread = {
    {
QT_MOC_LITERAL(0, 0, 19) // "iser::GenericThread"

    },
    "iser::GenericThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_iser__GenericThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void iser::GenericThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject iser::GenericThread::staticMetaObject = {
    { &ad::SvAbstractDeviceThread::staticMetaObject, qt_meta_stringdata_iser__GenericThread.data,
      qt_meta_data_iser__GenericThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *iser::GenericThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *iser::GenericThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_iser__GenericThread.stringdata0))
        return static_cast<void*>(const_cast< GenericThread*>(this));
    return ad::SvAbstractDeviceThread::qt_metacast(_clname);
}

int iser::GenericThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ad::SvAbstractDeviceThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_iser__UDPThread_t {
    QByteArrayData data[3];
    char stringdata0[22];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_iser__UDPThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_iser__UDPThread_t qt_meta_stringdata_iser__UDPThread = {
    {
QT_MOC_LITERAL(0, 0, 15), // "iser::UDPThread"
QT_MOC_LITERAL(1, 16, 4), // "stop"
QT_MOC_LITERAL(2, 21, 0) // ""

    },
    "iser::UDPThread\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_iser__UDPThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void iser::UDPThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UDPThread *_t = static_cast<UDPThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject iser::UDPThread::staticMetaObject = {
    { &iser::GenericThread::staticMetaObject, qt_meta_stringdata_iser__UDPThread.data,
      qt_meta_data_iser__UDPThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *iser::UDPThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *iser::UDPThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_iser__UDPThread.stringdata0))
        return static_cast<void*>(const_cast< UDPThread*>(this));
    return iser::GenericThread::qt_metacast(_clname);
}

int iser::UDPThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = iser::GenericThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
