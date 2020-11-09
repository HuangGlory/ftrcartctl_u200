/****************************************************************************
** Meta object code from reading C++ file 'ftr_ctr3speedctl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../ftr_ctr3speedctl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftr_ctr3speedctl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FTR_CTR3SpeedCtl_t {
    QByteArrayData data[52];
    char stringdata0[802];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FTR_CTR3SpeedCtl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FTR_CTR3SpeedCtl_t qt_meta_stringdata_FTR_CTR3SpeedCtl = {
    {
QT_MOC_LITERAL(0, 0, 16), // "FTR_CTR3SpeedCtl"
QT_MOC_LITERAL(1, 17, 20), // "toCalcCapacitySignal"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 7), // "voltage"
QT_MOC_LITERAL(4, 47, 19), // "SetToPushInWorkSlot"
QT_MOC_LITERAL(5, 67, 17), // "on_readoutputSlot"
QT_MOC_LITERAL(6, 85, 22), // "CreateStreamlitAppSlot"
QT_MOC_LITERAL(7, 108, 20), // "StartStreamlitUISlot"
QT_MOC_LITERAL(8, 129, 19), // "StopStreamlitUISlot"
QT_MOC_LITERAL(9, 149, 13), // "Time2LoopSlot"
QT_MOC_LITERAL(10, 163, 18), // "Timer2SendDataSlot"
QT_MOC_LITERAL(11, 182, 18), // "PauseToGoTimerSlot"
QT_MOC_LITERAL(12, 201, 18), // "RecoverOATimerSlot"
QT_MOC_LITERAL(13, 220, 12), // "ReadUARTSlot"
QT_MOC_LITERAL(14, 233, 18), // "UpdateVelocitySlot"
QT_MOC_LITERAL(15, 252, 13), // "MotorCtrlInfo"
QT_MOC_LITERAL(16, 266, 17), // "CartStateSyncSlot"
QT_MOC_LITERAL(17, 284, 11), // "CartState_e"
QT_MOC_LITERAL(18, 296, 9), // "CartState"
QT_MOC_LITERAL(19, 306, 17), // "WriteMainPipeSlot"
QT_MOC_LITERAL(20, 324, 9), // "cartState"
QT_MOC_LITERAL(21, 334, 25), // "WriteInfoToVisionPipeSlot"
QT_MOC_LITERAL(22, 360, 4), // "info"
QT_MOC_LITERAL(23, 365, 15), // "fileChangedSlot"
QT_MOC_LITERAL(24, 381, 4), // "path"
QT_MOC_LITERAL(25, 386, 16), // "WriteOutPipeSlot"
QT_MOC_LITERAL(26, 403, 3), // "str"
QT_MOC_LITERAL(27, 407, 17), // "UpdateVTKInfoSlot"
QT_MOC_LITERAL(28, 425, 9), // "VTKInfo_t"
QT_MOC_LITERAL(29, 435, 7), // "VTKInfo"
QT_MOC_LITERAL(30, 443, 17), // "UpdateVTPInfoSlot"
QT_MOC_LITERAL(31, 461, 9), // "VTPInfo_t"
QT_MOC_LITERAL(32, 471, 7), // "VTPInfo"
QT_MOC_LITERAL(33, 479, 19), // "UpdatePipeInputSlot"
QT_MOC_LITERAL(34, 499, 19), // "PNGButtonToggleSlot"
QT_MOC_LITERAL(35, 519, 19), // "SettingOAToggleSlot"
QT_MOC_LITERAL(36, 539, 27), // "SettingOAGlobalBaseJsonSlot"
QT_MOC_LITERAL(37, 567, 28), // "WriteWheelDiamToBaseJsonSlot"
QT_MOC_LITERAL(38, 596, 15), // "CalcArcInfoSlot"
QT_MOC_LITERAL(39, 612, 9), // "ArcInfo_t"
QT_MOC_LITERAL(40, 622, 11), // "PointAxis_t"
QT_MOC_LITERAL(41, 634, 2), // "p1"
QT_MOC_LITERAL(42, 637, 2), // "p2"
QT_MOC_LITERAL(43, 640, 2), // "p3"
QT_MOC_LITERAL(44, 643, 23), // "tcpServerConnectionSlot"
QT_MOC_LITERAL(45, 667, 17), // "tcpSocketReadSlot"
QT_MOC_LITERAL(46, 685, 24), // "tcpSocketSendMessageSlot"
QT_MOC_LITERAL(47, 710, 7), // "message"
QT_MOC_LITERAL(48, 718, 23), // "tcpSocketDisconnectSlot"
QT_MOC_LITERAL(49, 742, 28), // "lowPowerToShutDownSystemSlot"
QT_MOC_LITERAL(50, 771, 16), // "calcCapacitySlot"
QT_MOC_LITERAL(51, 788, 13) // "ResetWIFISlot"

    },
    "FTR_CTR3SpeedCtl\0toCalcCapacitySignal\0"
    "\0voltage\0SetToPushInWorkSlot\0"
    "on_readoutputSlot\0CreateStreamlitAppSlot\0"
    "StartStreamlitUISlot\0StopStreamlitUISlot\0"
    "Time2LoopSlot\0Timer2SendDataSlot\0"
    "PauseToGoTimerSlot\0RecoverOATimerSlot\0"
    "ReadUARTSlot\0UpdateVelocitySlot\0"
    "MotorCtrlInfo\0CartStateSyncSlot\0"
    "CartState_e\0CartState\0WriteMainPipeSlot\0"
    "cartState\0WriteInfoToVisionPipeSlot\0"
    "info\0fileChangedSlot\0path\0WriteOutPipeSlot\0"
    "str\0UpdateVTKInfoSlot\0VTKInfo_t\0VTKInfo\0"
    "UpdateVTPInfoSlot\0VTPInfo_t\0VTPInfo\0"
    "UpdatePipeInputSlot\0PNGButtonToggleSlot\0"
    "SettingOAToggleSlot\0SettingOAGlobalBaseJsonSlot\0"
    "WriteWheelDiamToBaseJsonSlot\0"
    "CalcArcInfoSlot\0ArcInfo_t\0PointAxis_t\0"
    "p1\0p2\0p3\0tcpServerConnectionSlot\0"
    "tcpSocketReadSlot\0tcpSocketSendMessageSlot\0"
    "message\0tcpSocketDisconnectSlot\0"
    "lowPowerToShutDownSystemSlot\0"
    "calcCapacitySlot\0ResetWIFISlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FTR_CTR3SpeedCtl[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  174,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  177,    2, 0x0a /* Public */,
       5,    0,  178,    2, 0x0a /* Public */,
       6,    0,  179,    2, 0x0a /* Public */,
       7,    0,  180,    2, 0x0a /* Public */,
       8,    0,  181,    2, 0x0a /* Public */,
       9,    0,  182,    2, 0x0a /* Public */,
      10,    0,  183,    2, 0x0a /* Public */,
      11,    0,  184,    2, 0x0a /* Public */,
      12,    0,  185,    2, 0x0a /* Public */,
      13,    0,  186,    2, 0x0a /* Public */,
      14,    1,  187,    2, 0x0a /* Public */,
      16,    1,  190,    2, 0x0a /* Public */,
      19,    1,  193,    2, 0x0a /* Public */,
      21,    1,  196,    2, 0x0a /* Public */,
      23,    1,  199,    2, 0x0a /* Public */,
      25,    1,  202,    2, 0x0a /* Public */,
      27,    1,  205,    2, 0x0a /* Public */,
      30,    1,  208,    2, 0x0a /* Public */,
      33,    1,  211,    2, 0x0a /* Public */,
      34,    0,  214,    2, 0x0a /* Public */,
      35,    1,  215,    2, 0x0a /* Public */,
      36,    0,  218,    2, 0x0a /* Public */,
      37,    0,  219,    2, 0x0a /* Public */,
      38,    3,  220,    2, 0x0a /* Public */,
      44,    0,  227,    2, 0x0a /* Public */,
      45,    0,  228,    2, 0x0a /* Public */,
      46,    1,  229,    2, 0x0a /* Public */,
      48,    0,  232,    2, 0x0a /* Public */,
      49,    0,  233,    2, 0x0a /* Public */,
      50,    1,  234,    2, 0x0a /* Public */,
      51,    0,  237,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,    2,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 17,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void, 0x80000000 | 31,   32,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   20,
    QMetaType::Bool,
    QMetaType::Bool,
    0x80000000 | 39, 0x80000000 | 40, 0x80000000 | 40, 0x80000000 | 40,   41,   42,   43,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   47,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,

       0        // eod
};

void FTR_CTR3SpeedCtl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FTR_CTR3SpeedCtl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toCalcCapacitySignal((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->SetToPushInWorkSlot(); break;
        case 2: _t->on_readoutputSlot(); break;
        case 3: _t->CreateStreamlitAppSlot(); break;
        case 4: _t->StartStreamlitUISlot(); break;
        case 5: _t->StopStreamlitUISlot(); break;
        case 6: _t->Time2LoopSlot(); break;
        case 7: _t->Timer2SendDataSlot(); break;
        case 8: _t->PauseToGoTimerSlot(); break;
        case 9: _t->RecoverOATimerSlot(); break;
        case 10: _t->ReadUARTSlot(); break;
        case 11: _t->UpdateVelocitySlot((*reinterpret_cast< MotorCtrlInfo(*)>(_a[1]))); break;
        case 12: _t->CartStateSyncSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 13: _t->WriteMainPipeSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 14: _t->WriteInfoToVisionPipeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->fileChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->WriteOutPipeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->UpdateVTKInfoSlot((*reinterpret_cast< VTKInfo_t(*)>(_a[1]))); break;
        case 18: _t->UpdateVTPInfoSlot((*reinterpret_cast< VTPInfo_t(*)>(_a[1]))); break;
        case 19: _t->UpdatePipeInputSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->PNGButtonToggleSlot(); break;
        case 21: _t->SettingOAToggleSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 22: { bool _r = _t->SettingOAGlobalBaseJsonSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 23: { bool _r = _t->WriteWheelDiamToBaseJsonSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 24: { ArcInfo_t _r = _t->CalcArcInfoSlot((*reinterpret_cast< PointAxis_t(*)>(_a[1])),(*reinterpret_cast< PointAxis_t(*)>(_a[2])),(*reinterpret_cast< PointAxis_t(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< ArcInfo_t*>(_a[0]) = std::move(_r); }  break;
        case 25: _t->tcpServerConnectionSlot(); break;
        case 26: _t->tcpSocketReadSlot(); break;
        case 27: _t->tcpSocketSendMessageSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->tcpSocketDisconnectSlot(); break;
        case 29: _t->lowPowerToShutDownSystemSlot(); break;
        case 30: _t->calcCapacitySlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 31: _t->ResetWIFISlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FTR_CTR3SpeedCtl::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FTR_CTR3SpeedCtl::toCalcCapacitySignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FTR_CTR3SpeedCtl::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_FTR_CTR3SpeedCtl.data,
    qt_meta_data_FTR_CTR3SpeedCtl,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FTR_CTR3SpeedCtl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FTR_CTR3SpeedCtl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FTR_CTR3SpeedCtl.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FTR_CTR3SpeedCtl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 32;
    }
    return _id;
}

// SIGNAL 0
void FTR_CTR3SpeedCtl::toCalcCapacitySignal(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
