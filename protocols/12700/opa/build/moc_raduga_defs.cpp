/****************************************************************************
** Meta object code from reading C++ file 'raduga_defs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../apak/imitator/raduga_imitator/src/raduga_defs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'raduga_defs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_raduga__SvAbstractSignalCollection_t {
    QByteArrayData data[1];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_raduga__SvAbstractSignalCollection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_raduga__SvAbstractSignalCollection_t qt_meta_stringdata_raduga__SvAbstractSignalCollection = {
    {
QT_MOC_LITERAL(0, 0, 34) // "raduga::SvAbstractSignalColle..."

    },
    "raduga::SvAbstractSignalCollection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_raduga__SvAbstractSignalCollection[] = {

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

void raduga::SvAbstractSignalCollection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject raduga::SvAbstractSignalCollection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_raduga__SvAbstractSignalCollection.data,
      qt_meta_data_raduga__SvAbstractSignalCollection,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *raduga::SvAbstractSignalCollection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *raduga::SvAbstractSignalCollection::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_raduga__SvAbstractSignalCollection.stringdata0))
        return static_cast<void*>(const_cast< SvAbstractSignalCollection*>(this));
    return QObject::qt_metacast(_clname);
}

int raduga::SvAbstractSignalCollection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
