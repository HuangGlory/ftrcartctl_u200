/****************************************************************************
** Meta object code from reading C++ file 'can.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ftrCartCtl_WDS/can.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'can.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_can_t {
    QByteArrayData data[11];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_can_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_can_t qt_meta_stringdata_can = {
    {
QT_MOC_LITERAL(0, 0, 3), // "can"
QT_MOC_LITERAL(1, 4, 28), // "ReceivedPayloadFromCanSignal"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "payload"
QT_MOC_LITERAL(4, 42, 25), // "processReceivedFramesSlot"
QT_MOC_LITERAL(5, 68, 20), // "processFramesWritten"
QT_MOC_LITERAL(6, 89, 5), // "count"
QT_MOC_LITERAL(7, 95, 14), // "CanDeviceState"
QT_MOC_LITERAL(8, 110, 32), // "QCanBusDevice::CanBusDeviceState"
QT_MOC_LITERAL(9, 143, 5), // "state"
QT_MOC_LITERAL(10, 149, 13) // "SendDataToCan"

    },
    "can\0ReceivedPayloadFromCanSignal\0\0"
    "payload\0processReceivedFramesSlot\0"
    "processFramesWritten\0count\0CanDeviceState\0"
    "QCanBusDevice::CanBusDeviceState\0state\0"
    "SendDataToCan"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_can[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   42,    2, 0x0a /* Public */,
       5,    1,   43,    2, 0x0a /* Public */,
       7,    1,   46,    2, 0x0a /* Public */,
      10,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,    6,
    QMetaType::UChar, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QByteArray,    3,

       0        // eod
};

void can::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<can *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ReceivedPayloadFromCanSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->processReceivedFramesSlot(); break;
        case 2: _t->processFramesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 3: { quint8 _r = _t->CanDeviceState((*reinterpret_cast< QCanBusDevice::CanBusDeviceState(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< quint8*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->SendDataToCan((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (can::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&can::ReceivedPayloadFromCanSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject can::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_can.data,
    qt_meta_data_can,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *can::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *can::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_can.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int can::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void can::ReceivedPayloadFromCanSignal(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
