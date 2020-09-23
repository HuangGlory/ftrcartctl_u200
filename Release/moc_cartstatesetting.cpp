/****************************************************************************
** Meta object code from reading C++ file 'cartstatesetting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../ftrCartCtl_WDS/cartstatesetting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cartstatesetting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CartStateSetting_t {
    QByteArrayData data[15];
    char stringdata0[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CartStateSetting_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CartStateSetting_t qt_meta_stringdata_CartStateSetting = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CartStateSetting"
QT_MOC_LITERAL(1, 17, 11), // "KeyISRSignl"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 24), // "BroadcastCartStateSignal"
QT_MOC_LITERAL(4, 55, 11), // "CartState_e"
QT_MOC_LITERAL(5, 67, 21), // "PNGButtonToggleSignal"
QT_MOC_LITERAL(6, 89, 21), // "SettingOAToggleSignal"
QT_MOC_LITERAL(7, 111, 13), // "LoopTimerSlot"
QT_MOC_LITERAL(8, 125, 25), // "LEDBlinkSpeedCtlTimerSlot"
QT_MOC_LITERAL(9, 151, 10), // "KeyISRSlot"
QT_MOC_LITERAL(10, 162, 20), // "SetCartStateExternal"
QT_MOC_LITERAL(11, 183, 23), // "SetInConfigModeFlagSlot"
QT_MOC_LITERAL(12, 207, 5), // "state"
QT_MOC_LITERAL(13, 213, 22), // "SetCameraReadyFlagSlot"
QT_MOC_LITERAL(14, 236, 16) // "SetVTKInIdleFlag"

    },
    "CartStateSetting\0KeyISRSignl\0\0"
    "BroadcastCartStateSignal\0CartState_e\0"
    "PNGButtonToggleSignal\0SettingOAToggleSignal\0"
    "LoopTimerSlot\0LEDBlinkSpeedCtlTimerSlot\0"
    "KeyISRSlot\0SetCartStateExternal\0"
    "SetInConfigModeFlagSlot\0state\0"
    "SetCameraReadyFlagSlot\0SetVTKInIdleFlag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CartStateSetting[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    1,   70,    2, 0x06 /* Public */,
       5,    0,   73,    2, 0x06 /* Public */,
       6,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   77,    2, 0x0a /* Public */,
       8,    0,   78,    2, 0x0a /* Public */,
       9,    0,   79,    2, 0x0a /* Public */,
      10,    1,   80,    2, 0x0a /* Public */,
      11,    1,   83,    2, 0x0a /* Public */,
      13,    1,   86,    2, 0x0a /* Public */,
      14,    1,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,

       0        // eod
};

void CartStateSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CartStateSetting *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->KeyISRSignl(); break;
        case 1: _t->BroadcastCartStateSignal((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 2: _t->PNGButtonToggleSignal(); break;
        case 3: _t->SettingOAToggleSignal((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 4: _t->LoopTimerSlot(); break;
        case 5: _t->LEDBlinkSpeedCtlTimerSlot(); break;
        case 6: _t->KeyISRSlot(); break;
        case 7: _t->SetCartStateExternal((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 8: _t->SetInConfigModeFlagSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->SetCameraReadyFlagSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->SetVTKInIdleFlag((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CartStateSetting::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CartStateSetting::KeyISRSignl)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CartStateSetting::*)(CartState_e );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CartStateSetting::BroadcastCartStateSignal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CartStateSetting::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CartStateSetting::PNGButtonToggleSignal)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CartStateSetting::*)(CartState_e );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CartStateSetting::SettingOAToggleSignal)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CartStateSetting::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CartStateSetting.data,
    qt_meta_data_CartStateSetting,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CartStateSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CartStateSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CartStateSetting.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CartStateSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CartStateSetting::KeyISRSignl()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CartStateSetting::BroadcastCartStateSignal(CartState_e _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CartStateSetting::PNGButtonToggleSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CartStateSetting::SettingOAToggleSignal(CartState_e _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
