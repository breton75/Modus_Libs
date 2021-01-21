/****************************************************************************
** Meta object code from reading C++ file 'sv_abstract_device.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Modus/global/device/sv_abstract_device.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_abstract_device.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_modus__SvAbstractDevice_t {
    QByteArrayData data[19];
    char stringdata0[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_modus__SvAbstractDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_modus__SvAbstractDevice_t qt_meta_stringdata_modus__SvAbstractDevice = {
    {
QT_MOC_LITERAL(0, 0, 23), // "modus::SvAbstractDevice"
QT_MOC_LITERAL(1, 24, 17), // "affirmDeviceAlive"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 17), // "inputBufferParsed"
QT_MOC_LITERAL(4, 61, 9), // "alive_age"
QT_MOC_LITERAL(5, 71, 17), // "outputBufferReady"
QT_MOC_LITERAL(6, 89, 7), // "message"
QT_MOC_LITERAL(7, 97, 3), // "msg"
QT_MOC_LITERAL(8, 101, 14), // "sv::log::Level"
QT_MOC_LITERAL(9, 116, 5), // "level"
QT_MOC_LITERAL(10, 122, 21), // "sv::log::MessageTypes"
QT_MOC_LITERAL(11, 144, 4), // "type"
QT_MOC_LITERAL(12, 149, 16), // "processInputData"
QT_MOC_LITERAL(13, 166, 17), // "processOutputData"
QT_MOC_LITERAL(14, 184, 15), // "validateSignals"
QT_MOC_LITERAL(15, 200, 5), // "queue"
QT_MOC_LITERAL(16, 206, 9), // "SvSignal*"
QT_MOC_LITERAL(17, 216, 6), // "signal"
QT_MOC_LITERAL(18, 223, 4) // "stop"

    },
    "modus::SvAbstractDevice\0affirmDeviceAlive\0"
    "\0inputBufferParsed\0alive_age\0"
    "outputBufferReady\0message\0msg\0"
    "sv::log::Level\0level\0sv::log::MessageTypes\0"
    "type\0processInputData\0processOutputData\0"
    "validateSignals\0queue\0SvSignal*\0signal\0"
    "stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_modus__SvAbstractDevice[] = {

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
       1,    0,   69,    2, 0x06 /* Public */,
       3,    1,   70,    2, 0x06 /* Public */,
       5,    0,   73,    2, 0x06 /* Public */,
       6,    3,   74,    2, 0x06 /* Public */,
       6,    2,   81,    2, 0x26 /* Public | MethodCloned */,
       6,    1,   86,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   89,    2, 0x0a /* Public */,
      13,    0,   90,    2, 0x0a /* Public */,
      14,    0,   91,    2, 0x0a /* Public */,
      15,    1,   92,    2, 0x0a /* Public */,
      18,    0,   95,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULongLong,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8, 0x80000000 | 10,    7,    9,   11,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    9,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,

       0        // eod
};

void modus::SvAbstractDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvAbstractDevice *_t = static_cast<SvAbstractDevice *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->affirmDeviceAlive(); break;
        case 1: _t->inputBufferParsed((*reinterpret_cast< const quint64(*)>(_a[1]))); break;
        case 2: _t->outputBufferReady(); break;
        case 3: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< sv::log::Level(*)>(_a[2])),(*reinterpret_cast< sv::log::MessageTypes(*)>(_a[3]))); break;
        case 4: _t->message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< sv::log::Level(*)>(_a[2]))); break;
        case 5: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->processInputData(); break;
        case 7: _t->processOutputData(); break;
        case 8: _t->validateSignals(); break;
        case 9: _t->queue((*reinterpret_cast< SvSignal*(*)>(_a[1]))); break;
        case 10: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SvSignal* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SvAbstractDevice::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SvAbstractDevice::affirmDeviceAlive)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SvAbstractDevice::*_t)(const quint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SvAbstractDevice::inputBufferParsed)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SvAbstractDevice::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SvAbstractDevice::outputBufferReady)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SvAbstractDevice::*_t)(const QString , sv::log::Level , sv::log::MessageTypes );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SvAbstractDevice::message)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject modus::SvAbstractDevice::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_modus__SvAbstractDevice.data,
      qt_meta_data_modus__SvAbstractDevice,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *modus::SvAbstractDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *modus::SvAbstractDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_modus__SvAbstractDevice.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int modus::SvAbstractDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void modus::SvAbstractDevice::affirmDeviceAlive()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void modus::SvAbstractDevice::inputBufferParsed(const quint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void modus::SvAbstractDevice::outputBufferReady()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void modus::SvAbstractDevice::message(const QString _t1, sv::log::Level _t2, sv::log::MessageTypes _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
