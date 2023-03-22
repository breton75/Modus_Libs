/****************************************************************************
** Meta object code from reading C++ file 'sv_tcp_server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/sv_tcp_server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sv_tcp_server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SvTcpServer_t {
    QByteArrayData data[22];
    char stringdata0[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SvTcpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SvTcpServer_t qt_meta_stringdata_SvTcpServer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SvTcpServer"
QT_MOC_LITERAL(1, 12, 5), // "start"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 4), // "read"
QT_MOC_LITERAL(4, 24, 5), // "write"
QT_MOC_LITERAL(5, 30, 12), // "modus::BUFF*"
QT_MOC_LITERAL(6, 43, 6), // "buffer"
QT_MOC_LITERAL(7, 50, 11), // "socketError"
QT_MOC_LITERAL(8, 62, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(9, 91, 3), // "err"
QT_MOC_LITERAL(10, 95, 12), // "stateChanged"
QT_MOC_LITERAL(11, 108, 28), // "QAbstractSocket::SocketState"
QT_MOC_LITERAL(12, 137, 5), // "state"
QT_MOC_LITERAL(13, 143, 7), // "newData"
QT_MOC_LITERAL(14, 151, 13), // "newConnection"
QT_MOC_LITERAL(15, 165, 11), // "disconected"
QT_MOC_LITERAL(16, 177, 12), // "emit_message"
QT_MOC_LITERAL(17, 190, 5), // "bytes"
QT_MOC_LITERAL(18, 196, 14), // "sv::log::Level"
QT_MOC_LITERAL(19, 211, 5), // "level"
QT_MOC_LITERAL(20, 217, 21), // "sv::log::MessageTypes"
QT_MOC_LITERAL(21, 239, 4) // "type"

    },
    "SvTcpServer\0start\0\0read\0write\0"
    "modus::BUFF*\0buffer\0socketError\0"
    "QAbstractSocket::SocketError\0err\0"
    "stateChanged\0QAbstractSocket::SocketState\0"
    "state\0newData\0newConnection\0disconected\0"
    "emit_message\0bytes\0sv::log::Level\0"
    "level\0sv::log::MessageTypes\0type"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SvTcpServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    0,   60,    2, 0x0a /* Public */,
       4,    1,   61,    2, 0x0a /* Public */,
       7,    1,   64,    2, 0x08 /* Private */,
      10,    1,   67,    2, 0x08 /* Private */,
      13,    0,   70,    2, 0x08 /* Private */,
      14,    0,   71,    2, 0x08 /* Private */,
      15,    0,   72,    2, 0x08 /* Private */,
      16,    3,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 18, 0x80000000 | 20,   17,   19,   21,

       0        // eod
};

void SvTcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SvTcpServer *_t = static_cast<SvTcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->start();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: _t->read(); break;
        case 2: _t->write((*reinterpret_cast< modus::BUFF*(*)>(_a[1]))); break;
        case 3: _t->socketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 4: _t->stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 5: _t->newData(); break;
        case 6: _t->newConnection(); break;
        case 7: _t->disconected(); break;
        case 8: _t->emit_message((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< sv::log::Level(*)>(_a[2])),(*reinterpret_cast< sv::log::MessageTypes(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketState >(); break;
            }
            break;
        }
    }
}

const QMetaObject SvTcpServer::staticMetaObject = {
    { &modus::SvAbstractInterface::staticMetaObject, qt_meta_stringdata_SvTcpServer.data,
      qt_meta_data_SvTcpServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SvTcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SvTcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SvTcpServer.stringdata0))
        return static_cast<void*>(const_cast< SvTcpServer*>(this));
    return modus::SvAbstractInterface::qt_metacast(_clname);
}

int SvTcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = modus::SvAbstractInterface::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
