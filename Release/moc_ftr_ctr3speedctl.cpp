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
    QByteArrayData data[46];
    char stringdata0[686];
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
QT_MOC_LITERAL(4, 47, 13), // "Time2LoopSlot"
QT_MOC_LITERAL(5, 61, 18), // "Timer2SendDataSlot"
QT_MOC_LITERAL(6, 80, 18), // "PauseToGoTimerSlot"
QT_MOC_LITERAL(7, 99, 18), // "RecoverOATimerSlot"
QT_MOC_LITERAL(8, 118, 12), // "ReadUARTSlot"
QT_MOC_LITERAL(9, 131, 18), // "UpdateVelocitySlot"
QT_MOC_LITERAL(10, 150, 13), // "MotorCtrlInfo"
QT_MOC_LITERAL(11, 164, 17), // "CartStateSyncSlot"
QT_MOC_LITERAL(12, 182, 11), // "CartState_e"
QT_MOC_LITERAL(13, 194, 9), // "CartState"
QT_MOC_LITERAL(14, 204, 17), // "WriteMainPipeSlot"
QT_MOC_LITERAL(15, 222, 9), // "cartState"
QT_MOC_LITERAL(16, 232, 25), // "WriteInfoToVisionPipeSlot"
QT_MOC_LITERAL(17, 258, 4), // "info"
QT_MOC_LITERAL(18, 263, 15), // "fileChangedSlot"
QT_MOC_LITERAL(19, 279, 4), // "path"
QT_MOC_LITERAL(20, 284, 16), // "WriteOutPipeSlot"
QT_MOC_LITERAL(21, 301, 3), // "str"
QT_MOC_LITERAL(22, 305, 17), // "UpdateVTKInfoSlot"
QT_MOC_LITERAL(23, 323, 9), // "VTKInfo_t"
QT_MOC_LITERAL(24, 333, 7), // "VTKInfo"
QT_MOC_LITERAL(25, 341, 17), // "UpdateVTPInfoSlot"
QT_MOC_LITERAL(26, 359, 9), // "VTPInfo_t"
QT_MOC_LITERAL(27, 369, 7), // "VTPInfo"
QT_MOC_LITERAL(28, 377, 19), // "UpdatePipeInputSlot"
QT_MOC_LITERAL(29, 397, 19), // "PNGButtonToggleSlot"
QT_MOC_LITERAL(30, 417, 19), // "SettingOAToggleSlot"
QT_MOC_LITERAL(31, 437, 27), // "SettingOAGlobalBaseJsonSlot"
QT_MOC_LITERAL(32, 465, 28), // "WriteWheelDiamToBaseJsonSlot"
QT_MOC_LITERAL(33, 494, 15), // "CalcArcInfoSlot"
QT_MOC_LITERAL(34, 510, 9), // "ArcInfo_t"
QT_MOC_LITERAL(35, 520, 11), // "PointAxis_t"
QT_MOC_LITERAL(36, 532, 2), // "p1"
QT_MOC_LITERAL(37, 535, 2), // "p2"
QT_MOC_LITERAL(38, 538, 2), // "p3"
QT_MOC_LITERAL(39, 541, 23), // "tcpServerConnectionSlot"
QT_MOC_LITERAL(40, 565, 17), // "tcpSocketReadSlot"
QT_MOC_LITERAL(41, 583, 24), // "tcpSocketSendMessageSlot"
QT_MOC_LITERAL(42, 608, 7), // "message"
QT_MOC_LITERAL(43, 616, 23), // "tcpSocketDisconnectSlot"
QT_MOC_LITERAL(44, 640, 28), // "lowPowerToShutDownSystemSlot"
QT_MOC_LITERAL(45, 669, 16) // "calcCapacitySlot"

    },
    "FTR_CTR3SpeedCtl\0toCalcCapacitySignal\0"
    "\0voltage\0Time2LoopSlot\0Timer2SendDataSlot\0"
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
    "calcCapacitySlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FTR_CTR3SpeedCtl[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  144,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  147,    2, 0x0a /* Public */,
       5,    0,  148,    2, 0x0a /* Public */,
       6,    0,  149,    2, 0x0a /* Public */,
       7,    0,  150,    2, 0x0a /* Public */,
       8,    0,  151,    2, 0x0a /* Public */,
       9,    1,  152,    2, 0x0a /* Public */,
      11,    1,  155,    2, 0x0a /* Public */,
      14,    1,  158,    2, 0x0a /* Public */,
      16,    1,  161,    2, 0x0a /* Public */,
      18,    1,  164,    2, 0x0a /* Public */,
      20,    1,  167,    2, 0x0a /* Public */,
      22,    1,  170,    2, 0x0a /* Public */,
      25,    1,  173,    2, 0x0a /* Public */,
      28,    1,  176,    2, 0x0a /* Public */,
      29,    0,  179,    2, 0x0a /* Public */,
      30,    1,  180,    2, 0x0a /* Public */,
      31,    0,  183,    2, 0x0a /* Public */,
      32,    0,  184,    2, 0x0a /* Public */,
      33,    3,  185,    2, 0x0a /* Public */,
      39,    0,  192,    2, 0x0a /* Public */,
      40,    0,  193,    2, 0x0a /* Public */,
      41,    1,  194,    2, 0x0a /* Public */,
      43,    0,  197,    2, 0x0a /* Public */,
      44,    0,  198,    2, 0x0a /* Public */,
      45,    1,  199,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 12,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void, 0x80000000 | 26,   27,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   15,
    QMetaType::Bool,
    QMetaType::Bool,
    0x80000000 | 34, 0x80000000 | 35, 0x80000000 | 35, 0x80000000 | 35,   36,   37,   38,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   42,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    3,

       0        // eod
};

void FTR_CTR3SpeedCtl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FTR_CTR3SpeedCtl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toCalcCapacitySignal((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->Time2LoopSlot(); break;
        case 2: _t->Timer2SendDataSlot(); break;
        case 3: _t->PauseToGoTimerSlot(); break;
        case 4: _t->RecoverOATimerSlot(); break;
        case 5: _t->ReadUARTSlot(); break;
        case 6: _t->UpdateVelocitySlot((*reinterpret_cast< MotorCtrlInfo(*)>(_a[1]))); break;
        case 7: _t->CartStateSyncSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 8: _t->WriteMainPipeSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 9: _t->WriteInfoToVisionPipeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->fileChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->WriteOutPipeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->UpdateVTKInfoSlot((*reinterpret_cast< VTKInfo_t(*)>(_a[1]))); break;
        case 13: _t->UpdateVTPInfoSlot((*reinterpret_cast< VTPInfo_t(*)>(_a[1]))); break;
        case 14: _t->UpdatePipeInputSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->PNGButtonToggleSlot(); break;
        case 16: _t->SettingOAToggleSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 17: { bool _r = _t->SettingOAGlobalBaseJsonSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 18: { bool _r = _t->WriteWheelDiamToBaseJsonSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 19: { ArcInfo_t _r = _t->CalcArcInfoSlot((*reinterpret_cast< PointAxis_t(*)>(_a[1])),(*reinterpret_cast< PointAxis_t(*)>(_a[2])),(*reinterpret_cast< PointAxis_t(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< ArcInfo_t*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->tcpServerConnectionSlot(); break;
        case 21: _t->tcpSocketReadSlot(); break;
        case 22: _t->tcpSocketSendMessageSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->tcpSocketDisconnectSlot(); break;
        case 24: _t->lowPowerToShutDownSystemSlot(); break;
        case 25: _t->calcCapacitySlot((*reinterpret_cast< double(*)>(_a[1]))); break;
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
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 26;
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
