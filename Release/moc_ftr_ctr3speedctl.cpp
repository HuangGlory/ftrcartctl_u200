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
    QByteArrayData data[64];
    char stringdata0[928];
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
QT_MOC_LITERAL(5, 67, 13), // "CreateMapSlot"
QT_MOC_LITERAL(6, 81, 7), // "station"
QT_MOC_LITERAL(7, 89, 4), // "dist"
QT_MOC_LITERAL(8, 94, 8), // "maxSpeed"
QT_MOC_LITERAL(9, 103, 4), // "face"
QT_MOC_LITERAL(10, 108, 14), // "isFirstStation"
QT_MOC_LITERAL(11, 123, 9), // "loadRount"
QT_MOC_LITERAL(12, 133, 8), // "fileName"
QT_MOC_LITERAL(13, 142, 24), // "GetActionBaseStationName"
QT_MOC_LITERAL(14, 167, 13), // "StationInfo_t"
QT_MOC_LITERAL(15, 181, 4), // "name"
QT_MOC_LITERAL(16, 186, 6), // "Repeat"
QT_MOC_LITERAL(17, 193, 17), // "on_readoutputSlot"
QT_MOC_LITERAL(18, 211, 22), // "CreateStreamlitAppSlot"
QT_MOC_LITERAL(19, 234, 20), // "StartStreamlitUISlot"
QT_MOC_LITERAL(20, 255, 19), // "StopStreamlitUISlot"
QT_MOC_LITERAL(21, 275, 13), // "Time2LoopSlot"
QT_MOC_LITERAL(22, 289, 18), // "Timer2SendDataSlot"
QT_MOC_LITERAL(23, 308, 18), // "PauseToGoTimerSlot"
QT_MOC_LITERAL(24, 327, 18), // "RecoverOATimerSlot"
QT_MOC_LITERAL(25, 346, 12), // "ReadUARTSlot"
QT_MOC_LITERAL(26, 359, 18), // "UpdateVelocitySlot"
QT_MOC_LITERAL(27, 378, 13), // "MotorCtrlInfo"
QT_MOC_LITERAL(28, 392, 17), // "CartStateSyncSlot"
QT_MOC_LITERAL(29, 410, 11), // "CartState_e"
QT_MOC_LITERAL(30, 422, 9), // "CartState"
QT_MOC_LITERAL(31, 432, 17), // "WriteMainPipeSlot"
QT_MOC_LITERAL(32, 450, 9), // "cartState"
QT_MOC_LITERAL(33, 460, 25), // "WriteInfoToVisionPipeSlot"
QT_MOC_LITERAL(34, 486, 4), // "info"
QT_MOC_LITERAL(35, 491, 15), // "fileChangedSlot"
QT_MOC_LITERAL(36, 507, 4), // "path"
QT_MOC_LITERAL(37, 512, 16), // "WriteOutPipeSlot"
QT_MOC_LITERAL(38, 529, 3), // "str"
QT_MOC_LITERAL(39, 533, 17), // "UpdateVTKInfoSlot"
QT_MOC_LITERAL(40, 551, 9), // "VTKInfo_t"
QT_MOC_LITERAL(41, 561, 7), // "VTKInfo"
QT_MOC_LITERAL(42, 569, 17), // "UpdateVTPInfoSlot"
QT_MOC_LITERAL(43, 587, 9), // "VTPInfo_t"
QT_MOC_LITERAL(44, 597, 7), // "VTPInfo"
QT_MOC_LITERAL(45, 605, 19), // "UpdatePipeInputSlot"
QT_MOC_LITERAL(46, 625, 19), // "PNGButtonToggleSlot"
QT_MOC_LITERAL(47, 645, 19), // "SettingOAToggleSlot"
QT_MOC_LITERAL(48, 665, 27), // "SettingOAGlobalBaseJsonSlot"
QT_MOC_LITERAL(49, 693, 28), // "WriteWheelDiamToBaseJsonSlot"
QT_MOC_LITERAL(50, 722, 15), // "CalcArcInfoSlot"
QT_MOC_LITERAL(51, 738, 9), // "ArcInfo_t"
QT_MOC_LITERAL(52, 748, 11), // "PointAxis_t"
QT_MOC_LITERAL(53, 760, 2), // "p1"
QT_MOC_LITERAL(54, 763, 2), // "p2"
QT_MOC_LITERAL(55, 766, 2), // "p3"
QT_MOC_LITERAL(56, 769, 23), // "tcpServerConnectionSlot"
QT_MOC_LITERAL(57, 793, 17), // "tcpSocketReadSlot"
QT_MOC_LITERAL(58, 811, 24), // "tcpSocketSendMessageSlot"
QT_MOC_LITERAL(59, 836, 7), // "message"
QT_MOC_LITERAL(60, 844, 23), // "tcpSocketDisconnectSlot"
QT_MOC_LITERAL(61, 868, 28), // "lowPowerToShutDownSystemSlot"
QT_MOC_LITERAL(62, 897, 16), // "calcCapacitySlot"
QT_MOC_LITERAL(63, 914, 13) // "ResetWIFISlot"

    },
    "FTR_CTR3SpeedCtl\0toCalcCapacitySignal\0"
    "\0voltage\0SetToPushInWorkSlot\0CreateMapSlot\0"
    "station\0dist\0maxSpeed\0face\0isFirstStation\0"
    "loadRount\0fileName\0GetActionBaseStationName\0"
    "StationInfo_t\0name\0Repeat\0on_readoutputSlot\0"
    "CreateStreamlitAppSlot\0StartStreamlitUISlot\0"
    "StopStreamlitUISlot\0Time2LoopSlot\0"
    "Timer2SendDataSlot\0PauseToGoTimerSlot\0"
    "RecoverOATimerSlot\0ReadUARTSlot\0"
    "UpdateVelocitySlot\0MotorCtrlInfo\0"
    "CartStateSyncSlot\0CartState_e\0CartState\0"
    "WriteMainPipeSlot\0cartState\0"
    "WriteInfoToVisionPipeSlot\0info\0"
    "fileChangedSlot\0path\0WriteOutPipeSlot\0"
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
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  189,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  192,    2, 0x0a /* Public */,
       5,    5,  193,    2, 0x0a /* Public */,
      11,    1,  204,    2, 0x0a /* Public */,
      13,    2,  207,    2, 0x0a /* Public */,
      17,    0,  212,    2, 0x0a /* Public */,
      18,    0,  213,    2, 0x0a /* Public */,
      19,    0,  214,    2, 0x0a /* Public */,
      20,    0,  215,    2, 0x0a /* Public */,
      21,    0,  216,    2, 0x0a /* Public */,
      22,    0,  217,    2, 0x0a /* Public */,
      23,    0,  218,    2, 0x0a /* Public */,
      24,    0,  219,    2, 0x0a /* Public */,
      25,    0,  220,    2, 0x0a /* Public */,
      26,    1,  221,    2, 0x0a /* Public */,
      28,    1,  224,    2, 0x0a /* Public */,
      31,    1,  227,    2, 0x0a /* Public */,
      33,    1,  230,    2, 0x0a /* Public */,
      35,    1,  233,    2, 0x0a /* Public */,
      37,    1,  236,    2, 0x0a /* Public */,
      39,    1,  239,    2, 0x0a /* Public */,
      42,    1,  242,    2, 0x0a /* Public */,
      45,    1,  245,    2, 0x0a /* Public */,
      46,    0,  248,    2, 0x0a /* Public */,
      47,    1,  249,    2, 0x0a /* Public */,
      48,    0,  252,    2, 0x0a /* Public */,
      49,    0,  253,    2, 0x0a /* Public */,
      50,    3,  254,    2, 0x0a /* Public */,
      56,    0,  261,    2, 0x0a /* Public */,
      57,    0,  262,    2, 0x0a /* Public */,
      58,    1,  263,    2, 0x0a /* Public */,
      60,    0,  266,    2, 0x0a /* Public */,
      61,    0,  267,    2, 0x0a /* Public */,
      62,    1,  268,    2, 0x0a /* Public */,
      63,    0,  271,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort, QMetaType::Short, QMetaType::Bool, QMetaType::Bool,    6,    7,    8,    9,   10,
    QMetaType::Void, QMetaType::QString,   12,
    0x80000000 | 14, QMetaType::QString, QMetaType::Bool,   15,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 27,    2,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void, 0x80000000 | 29,   32,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::Void, QMetaType::QString,   36,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void, 0x80000000 | 40,   41,
    QMetaType::Void, 0x80000000 | 43,   44,
    QMetaType::Void, QMetaType::QString,   38,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   32,
    QMetaType::Bool,
    QMetaType::Bool,
    0x80000000 | 51, 0x80000000 | 52, 0x80000000 | 52, 0x80000000 | 52,   53,   54,   55,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   59,
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
        case 2: _t->CreateMapSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< qint16(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 3: _t->loadRount((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: { StationInfo_t _r = _t->GetActionBaseStationName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< StationInfo_t*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->on_readoutputSlot(); break;
        case 6: _t->CreateStreamlitAppSlot(); break;
        case 7: _t->StartStreamlitUISlot(); break;
        case 8: _t->StopStreamlitUISlot(); break;
        case 9: _t->Time2LoopSlot(); break;
        case 10: _t->Timer2SendDataSlot(); break;
        case 11: _t->PauseToGoTimerSlot(); break;
        case 12: _t->RecoverOATimerSlot(); break;
        case 13: _t->ReadUARTSlot(); break;
        case 14: _t->UpdateVelocitySlot((*reinterpret_cast< MotorCtrlInfo(*)>(_a[1]))); break;
        case 15: _t->CartStateSyncSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 16: _t->WriteMainPipeSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 17: _t->WriteInfoToVisionPipeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->fileChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->WriteOutPipeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: _t->UpdateVTKInfoSlot((*reinterpret_cast< VTKInfo_t(*)>(_a[1]))); break;
        case 21: _t->UpdateVTPInfoSlot((*reinterpret_cast< VTPInfo_t(*)>(_a[1]))); break;
        case 22: _t->UpdatePipeInputSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->PNGButtonToggleSlot(); break;
        case 24: _t->SettingOAToggleSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 25: { bool _r = _t->SettingOAGlobalBaseJsonSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 26: { bool _r = _t->WriteWheelDiamToBaseJsonSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 27: { ArcInfo_t _r = _t->CalcArcInfoSlot((*reinterpret_cast< PointAxis_t(*)>(_a[1])),(*reinterpret_cast< PointAxis_t(*)>(_a[2])),(*reinterpret_cast< PointAxis_t(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< ArcInfo_t*>(_a[0]) = std::move(_r); }  break;
        case 28: _t->tcpServerConnectionSlot(); break;
        case 29: _t->tcpSocketReadSlot(); break;
        case 30: _t->tcpSocketSendMessageSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->tcpSocketDisconnectSlot(); break;
        case 32: _t->lowPowerToShutDownSystemSlot(); break;
        case 33: _t->calcCapacitySlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 34: _t->ResetWIFISlot(); break;
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
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 35;
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
