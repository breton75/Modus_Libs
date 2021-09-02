/****************************************************************************
** Meta object code from reading C++ file 'sv_log_server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../interacts/log_server/src/sv_log_server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_log_server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_httplog__SvHttpEventlog_t {
    QByteArrayData data[12];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_httplog__SvHttpEventlog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_httplog__SvHttpEventlog_t qt_meta_stringdata_httplog__SvHttpEventlog = {
    {
QT_MOC_LITERAL(0, 0, 23), // "httplog::SvHttpEventlog"
QT_MOC_LITERAL(1, 24, 4), // "stop"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 13), // "newConnection"
QT_MOC_LITERAL(4, 44, 14), // "processRequest"
QT_MOC_LITERAL(5, 59, 18), // "socketDisconnected"
QT_MOC_LITERAL(6, 78, 11), // "messageSlot"
QT_MOC_LITERAL(7, 90, 6), // "entity"
QT_MOC_LITERAL(8, 97, 2), // "id"
QT_MOC_LITERAL(9, 100, 4), // "type"
QT_MOC_LITERAL(10, 105, 4), // "time"
QT_MOC_LITERAL(11, 110, 7) // "message"

    },
    "httplog::SvHttpEventlog\0stop\0\0"
    "newConnection\0processRequest\0"
    "socketDisconnected\0messageSlot\0entity\0"
    "id\0type\0time\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_httplog__SvHttpEventlog[] = {

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
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    5,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    8,    9,   10,   11,

       0        // eod
};

void httplog::SvHttpEventlog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvHttpEventlog *_t = static_cast<SvHttpEventlog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stop(); break;
        case 1: _t->newConnection(); break;
        case 2: _t->processRequest(); break;
        case 3: _t->socketDisconnected(); break;
        case 4: _t->messageSlot((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObject httplog::SvHttpEventlog::staticMetaObject = {
    { &modus::SvAbstractInteract::staticMetaObject, qt_meta_stringdata_httplog__SvHttpEventlog.data,
      qt_meta_data_httplog__SvHttpEventlog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *httplog::SvHttpEventlog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *httplog::SvHttpEventlog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_httplog__SvHttpEventlog.stringdata0))
        return static_cast<void*>(const_cast< SvHttpEventlog*>(this));
    return modus::SvAbstractInteract::qt_metacast(_clname);
}

int httplog::SvHttpEventlog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = modus::SvAbstractInteract::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
