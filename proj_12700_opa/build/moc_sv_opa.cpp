/****************************************************************************
** Meta object code from reading C++ file 'sv_opa.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/sv_opa.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_opa.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_opa__SvOPA_t {
    QByteArrayData data[4];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_opa__SvOPA_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_opa__SvOPA_t qt_meta_stringdata_opa__SvOPA = {
    {
QT_MOC_LITERAL(0, 0, 10), // "opa::SvOPA"
QT_MOC_LITERAL(1, 11, 10), // "stopThread"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 12) // "deleteThread"

    },
    "opa::SvOPA\0stopThread\0\0deleteThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_opa__SvOPA[] = {

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

void opa::SvOPA::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvOPA *_t = static_cast<SvOPA *>(_o);
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
            typedef void (SvOPA::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SvOPA::stopThread)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject opa::SvOPA::staticMetaObject = {
    { &ad::SvAbstractDevice::staticMetaObject, qt_meta_stringdata_opa__SvOPA.data,
      qt_meta_data_opa__SvOPA,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *opa::SvOPA::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *opa::SvOPA::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_opa__SvOPA.stringdata0))
        return static_cast<void*>(const_cast< SvOPA*>(this));
    return ad::SvAbstractDevice::qt_metacast(_clname);
}

int opa::SvOPA::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void opa::SvOPA::stopThread()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_opa__GenericThread_t {
    QByteArrayData data[1];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_opa__GenericThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_opa__GenericThread_t qt_meta_stringdata_opa__GenericThread = {
    {
QT_MOC_LITERAL(0, 0, 18) // "opa::GenericThread"

    },
    "opa::GenericThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_opa__GenericThread[] = {

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

void opa::GenericThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject opa::GenericThread::staticMetaObject = {
    { &ad::SvAbstractDeviceThread::staticMetaObject, qt_meta_stringdata_opa__GenericThread.data,
      qt_meta_data_opa__GenericThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *opa::GenericThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *opa::GenericThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_opa__GenericThread.stringdata0))
        return static_cast<void*>(const_cast< GenericThread*>(this));
    return ad::SvAbstractDeviceThread::qt_metacast(_clname);
}

int opa::GenericThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ad::SvAbstractDeviceThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_opa__UDPThread_t {
    QByteArrayData data[3];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_opa__UDPThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_opa__UDPThread_t qt_meta_stringdata_opa__UDPThread = {
    {
QT_MOC_LITERAL(0, 0, 14), // "opa::UDPThread"
QT_MOC_LITERAL(1, 15, 4), // "stop"
QT_MOC_LITERAL(2, 20, 0) // ""

    },
    "opa::UDPThread\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_opa__UDPThread[] = {

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

void opa::UDPThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
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

const QMetaObject opa::UDPThread::staticMetaObject = {
    { &opa::GenericThread::staticMetaObject, qt_meta_stringdata_opa__UDPThread.data,
      qt_meta_data_opa__UDPThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *opa::UDPThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *opa::UDPThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_opa__UDPThread.stringdata0))
        return static_cast<void*>(const_cast< UDPThread*>(this));
    return opa::GenericThread::qt_metacast(_clname);
}

int opa::UDPThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = opa::GenericThread::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_opa__SerialThread_t {
    QByteArrayData data[3];
    char stringdata0[24];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_opa__SerialThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_opa__SerialThread_t qt_meta_stringdata_opa__SerialThread = {
    {
QT_MOC_LITERAL(0, 0, 17), // "opa::SerialThread"
QT_MOC_LITERAL(1, 18, 4), // "stop"
QT_MOC_LITERAL(2, 23, 0) // ""

    },
    "opa::SerialThread\0stop\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_opa__SerialThread[] = {

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

void opa::SerialThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialThread *_t = static_cast<SerialThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject opa::SerialThread::staticMetaObject = {
    { &opa::GenericThread::staticMetaObject, qt_meta_stringdata_opa__SerialThread.data,
      qt_meta_data_opa__SerialThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *opa::SerialThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *opa::SerialThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_opa__SerialThread.stringdata0))
        return static_cast<void*>(const_cast< SerialThread*>(this));
    return opa::GenericThread::qt_metacast(_clname);
}

int opa::SerialThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = opa::GenericThread::qt_metacall(_c, _id, _a);
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
