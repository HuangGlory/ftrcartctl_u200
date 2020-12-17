/****************************************************************************
** Meta object code from reading C++ file 'cartstatesetting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../cartstatesetting.h"
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
    QByteArrayData data[18];
    char stringdata0[311];
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
QT_MOC_LITERAL(7, 111, 21), // "SetToPushInWorkSignal"
QT_MOC_LITERAL(8, 133, 13), // "LoopTimerSlot"
QT_MOC_LITERAL(9, 147, 25), // "LEDBlinkSpeedCtlTimerSlot"
QT_MOC_LITERAL(10, 173, 10), // "KeyISRSlot"
QT_MOC_LITERAL(11, 184, 20), // "SetCartStateExternal"
QT_MOC_LITERAL(12, 205, 23), // "SetInConfigModeFlagSlot"
QT_MOC_LITERAL(13, 229, 5), // "state"
QT_MOC_LITERAL(14, 235, 22), // "SetCameraReadyFlagSlot"
QT_MOC_LITERAL(15, 258, 16), // "SetVTKInIdleFlag"
QT_MOC_LITERAL(16, 275, 17), // "SetVTKOAStateFlag"
QT_MOC_LITERAL(17, 293, 17) // "SetVTPOAStateFlag"

    },
    "CartStateSetting\0KeyISRSignl\0\0"
    "BroadcastCartStateSignal\0CartState_e\0"
    "PNGButtonToggleSignal\0SettingOAToggleSignal\0"
    "SetToPushInWorkSignal\0LoopTimerSlot\0"
    "LEDBlinkSpeedCtlTimerSlot\0KeyISRSlot\0"
    "SetCartStateExternal\0SetInConfigModeFlagSlot\0"
    "state\0SetCameraReadyFlagSlot\0"
    "SetVTKInIdleFlag\0SetVTKOAStateFlag\0"
    "SetVTPOAStateFlag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CartStateSetting[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    1,   85,    2, 0x06 /* Public */,
       5,    0,   88,    2, 0x06 /* Public */,
       6,    1,   89,    2, 0x06 /* Public */,
       7,    0,   92,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   93,    2, 0x0a /* Public */,
       9,    0,   94,    2, 0x0a /* Public */,
      10,    0,   95,    2, 0x0a /* Public */,
      11,    1,   96,    2, 0x0a /* Public */,
      12,    1,   99,    2, 0x0a /* Public */,
      14,    1,  102,    2, 0x0a /* Public */,
      15,    1,  105,    2, 0x0a /* Public */,
      16,    1,  108,    2, 0x0a /* Public */,
      17,    1,  111,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,

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
        case 4: _t->SetToPushInWorkSignal(); break;
        case 5: _t->LoopTimerSlot(); break;
        case 6: _t->LEDBlinkSpeedCtlTimerSlot(); break;
        case 7: _t->KeyISRSlot(); break;
        case 8: _t->SetCartStateExternal((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 9: _t->SetInConfigModeFlagSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->SetCameraReadyFlagSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->SetVTKInIdleFlag((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->SetVTKOAStateFlag((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->SetVTPOAStateFlag((*reinterpret_cast< bool(*)>(_a[1]))); break;
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
        {
            using _t = void (CartStateSetting::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CartStateSetting::SetToPushInWorkSignal)) {
                *result = 4;
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
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
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

// SIGNAL 4
void CartStateSetting::SetToPushInWorkSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
