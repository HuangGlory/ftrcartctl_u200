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
    QByteArrayData data[85];
    char stringdata0[1228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FTR_CTR3SpeedCtl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FTR_CTR3SpeedCtl_t qt_meta_stringdata_FTR_CTR3SpeedCtl = {
    {
QT_MOC_LITERAL(0, 0, 16), // "FTR_CTR3SpeedCtl"
QT_MOC_LITERAL(1, 17, 12), // "ServerRecved"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 7), // "qintptr"
QT_MOC_LITERAL(4, 39, 11), // "QTcpSocket*"
QT_MOC_LITERAL(5, 51, 20), // "toCalcCapacitySignal"
QT_MOC_LITERAL(6, 72, 7), // "voltage"
QT_MOC_LITERAL(7, 80, 14), // "CheckRTLogSize"
QT_MOC_LITERAL(8, 95, 16), // "setRtlogFileName"
QT_MOC_LITERAL(9, 112, 8), // "fileName"
QT_MOC_LITERAL(10, 121, 19), // "SetToPushInWorkSlot"
QT_MOC_LITERAL(11, 141, 13), // "CreateMapSlot"
QT_MOC_LITERAL(12, 155, 7), // "station"
QT_MOC_LITERAL(13, 163, 4), // "dist"
QT_MOC_LITERAL(14, 168, 8), // "maxSpeed"
QT_MOC_LITERAL(15, 177, 4), // "face"
QT_MOC_LITERAL(16, 182, 14), // "isFirstStation"
QT_MOC_LITERAL(17, 197, 9), // "loadRount"
QT_MOC_LITERAL(18, 207, 24), // "GetActionBaseStationName"
QT_MOC_LITERAL(19, 232, 13), // "StationInfo_t"
QT_MOC_LITERAL(20, 246, 4), // "name"
QT_MOC_LITERAL(21, 251, 6), // "Repeat"
QT_MOC_LITERAL(22, 258, 17), // "on_readoutputSlot"
QT_MOC_LITERAL(23, 276, 22), // "CreateStreamlitAppSlot"
QT_MOC_LITERAL(24, 299, 20), // "StartStreamlitUISlot"
QT_MOC_LITERAL(25, 320, 19), // "StopStreamlitUISlot"
QT_MOC_LITERAL(26, 340, 11), // "get_ip_addr"
QT_MOC_LITERAL(27, 352, 12), // "getCartModel"
QT_MOC_LITERAL(28, 365, 11), // "getHostName"
QT_MOC_LITERAL(29, 377, 26), // "on_frpServerReadOutputSlot"
QT_MOC_LITERAL(30, 404, 24), // "on_GetSSIDReadOutputSlot"
QT_MOC_LITERAL(31, 429, 13), // "getSSIDScript"
QT_MOC_LITERAL(32, 443, 14), // "startFRPServer"
QT_MOC_LITERAL(33, 458, 13), // "killFRPServer"
QT_MOC_LITERAL(34, 472, 13), // "Time2LoopSlot"
QT_MOC_LITERAL(35, 486, 18), // "Timer2SendDataSlot"
QT_MOC_LITERAL(36, 505, 18), // "PauseToGoTimerSlot"
QT_MOC_LITERAL(37, 524, 18), // "RecoverOATimerSlot"
QT_MOC_LITERAL(38, 543, 12), // "ReadUARTSlot"
QT_MOC_LITERAL(39, 556, 18), // "UpdateVelocitySlot"
QT_MOC_LITERAL(40, 575, 13), // "MotorCtrlInfo"
QT_MOC_LITERAL(41, 589, 17), // "CartStateSyncSlot"
QT_MOC_LITERAL(42, 607, 11), // "CartState_e"
QT_MOC_LITERAL(43, 619, 9), // "CartState"
QT_MOC_LITERAL(44, 629, 17), // "WriteMainPipeSlot"
QT_MOC_LITERAL(45, 647, 9), // "cartState"
QT_MOC_LITERAL(46, 657, 25), // "WriteInfoToVisionPipeSlot"
QT_MOC_LITERAL(47, 683, 4), // "info"
QT_MOC_LITERAL(48, 688, 15), // "fileChangedSlot"
QT_MOC_LITERAL(49, 704, 4), // "path"
QT_MOC_LITERAL(50, 709, 16), // "WriteOutPipeSlot"
QT_MOC_LITERAL(51, 726, 3), // "str"
QT_MOC_LITERAL(52, 730, 17), // "UpdateVTKInfoSlot"
QT_MOC_LITERAL(53, 748, 9), // "VTKInfo_t"
QT_MOC_LITERAL(54, 758, 7), // "VTKInfo"
QT_MOC_LITERAL(55, 766, 17), // "UpdateVTPInfoSlot"
QT_MOC_LITERAL(56, 784, 9), // "VTPInfo_t"
QT_MOC_LITERAL(57, 794, 7), // "VTPInfo"
QT_MOC_LITERAL(58, 802, 19), // "UpdatePipeInputSlot"
QT_MOC_LITERAL(59, 822, 19), // "PNGButtonToggleSlot"
QT_MOC_LITERAL(60, 842, 19), // "SettingOAToggleSlot"
QT_MOC_LITERAL(61, 862, 27), // "SettingOAGlobalBaseJsonSlot"
QT_MOC_LITERAL(62, 890, 28), // "WriteWheelDiamToBaseJsonSlot"
QT_MOC_LITERAL(63, 919, 15), // "CalcArcInfoSlot"
QT_MOC_LITERAL(64, 935, 9), // "ArcInfo_t"
QT_MOC_LITERAL(65, 945, 11), // "PointAxis_t"
QT_MOC_LITERAL(66, 957, 2), // "p1"
QT_MOC_LITERAL(67, 960, 2), // "p2"
QT_MOC_LITERAL(68, 963, 2), // "p3"
QT_MOC_LITERAL(69, 966, 15), // "ClientConnected"
QT_MOC_LITERAL(70, 982, 6), // "handle"
QT_MOC_LITERAL(71, 989, 6), // "socket"
QT_MOC_LITERAL(72, 996, 18), // "ClientDisconnected"
QT_MOC_LITERAL(73, 1015, 16), // "ServerRecvedSlot"
QT_MOC_LITERAL(74, 1032, 4), // "data"
QT_MOC_LITERAL(75, 1037, 17), // "tcpSocketReadSlot"
QT_MOC_LITERAL(76, 1055, 6), // "rxData"
QT_MOC_LITERAL(77, 1062, 24), // "tcpSocketSendMessageSlot"
QT_MOC_LITERAL(78, 1087, 7), // "message"
QT_MOC_LITERAL(79, 1095, 32), // "tcpSocketSendMessageToDeviceSlot"
QT_MOC_LITERAL(80, 1128, 14), // "updSendMessage"
QT_MOC_LITERAL(81, 1143, 28), // "lowPowerToShutDownSystemSlot"
QT_MOC_LITERAL(82, 1172, 16), // "calcCapacitySlot"
QT_MOC_LITERAL(83, 1189, 13), // "ResetWIFISlot"
QT_MOC_LITERAL(84, 1203, 24) // "getVisionVersionInfoSlot"

    },
    "FTR_CTR3SpeedCtl\0ServerRecved\0\0qintptr\0"
    "QTcpSocket*\0toCalcCapacitySignal\0"
    "voltage\0CheckRTLogSize\0setRtlogFileName\0"
    "fileName\0SetToPushInWorkSlot\0CreateMapSlot\0"
    "station\0dist\0maxSpeed\0face\0isFirstStation\0"
    "loadRount\0GetActionBaseStationName\0"
    "StationInfo_t\0name\0Repeat\0on_readoutputSlot\0"
    "CreateStreamlitAppSlot\0StartStreamlitUISlot\0"
    "StopStreamlitUISlot\0get_ip_addr\0"
    "getCartModel\0getHostName\0"
    "on_frpServerReadOutputSlot\0"
    "on_GetSSIDReadOutputSlot\0getSSIDScript\0"
    "startFRPServer\0killFRPServer\0Time2LoopSlot\0"
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
    "p1\0p2\0p3\0ClientConnected\0handle\0socket\0"
    "ClientDisconnected\0ServerRecvedSlot\0"
    "data\0tcpSocketReadSlot\0rxData\0"
    "tcpSocketSendMessageSlot\0message\0"
    "tcpSocketSendMessageToDeviceSlot\0"
    "updSendMessage\0lowPowerToShutDownSystemSlot\0"
    "calcCapacitySlot\0ResetWIFISlot\0"
    "getVisionVersionInfoSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FTR_CTR3SpeedCtl[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      50,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,  264,    2, 0x06 /* Public */,
       5,    1,  271,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  274,    2, 0x0a /* Public */,
       8,    1,  275,    2, 0x0a /* Public */,
      10,    0,  278,    2, 0x0a /* Public */,
      11,    5,  279,    2, 0x0a /* Public */,
      17,    1,  290,    2, 0x0a /* Public */,
      18,    2,  293,    2, 0x0a /* Public */,
      22,    0,  298,    2, 0x0a /* Public */,
      23,    0,  299,    2, 0x0a /* Public */,
      24,    0,  300,    2, 0x0a /* Public */,
      25,    0,  301,    2, 0x0a /* Public */,
      26,    0,  302,    2, 0x0a /* Public */,
      27,    0,  303,    2, 0x0a /* Public */,
      28,    0,  304,    2, 0x0a /* Public */,
      29,    0,  305,    2, 0x0a /* Public */,
      30,    0,  306,    2, 0x0a /* Public */,
      31,    0,  307,    2, 0x0a /* Public */,
      32,    0,  308,    2, 0x0a /* Public */,
      33,    0,  309,    2, 0x0a /* Public */,
      34,    0,  310,    2, 0x0a /* Public */,
      35,    0,  311,    2, 0x0a /* Public */,
      36,    0,  312,    2, 0x0a /* Public */,
      37,    0,  313,    2, 0x0a /* Public */,
      38,    0,  314,    2, 0x0a /* Public */,
      39,    1,  315,    2, 0x0a /* Public */,
      41,    1,  318,    2, 0x0a /* Public */,
      44,    1,  321,    2, 0x0a /* Public */,
      46,    1,  324,    2, 0x0a /* Public */,
      48,    1,  327,    2, 0x0a /* Public */,
      50,    1,  330,    2, 0x0a /* Public */,
      52,    1,  333,    2, 0x0a /* Public */,
      55,    1,  336,    2, 0x0a /* Public */,
      58,    1,  339,    2, 0x0a /* Public */,
      59,    0,  342,    2, 0x0a /* Public */,
      60,    1,  343,    2, 0x0a /* Public */,
      61,    0,  346,    2, 0x0a /* Public */,
      62,    0,  347,    2, 0x0a /* Public */,
      63,    3,  348,    2, 0x0a /* Public */,
      69,    2,  355,    2, 0x0a /* Public */,
      72,    1,  360,    2, 0x0a /* Public */,
      73,    3,  363,    2, 0x0a /* Public */,
      75,    1,  370,    2, 0x0a /* Public */,
      77,    1,  373,    2, 0x0a /* Public */,
      79,    1,  376,    2, 0x0a /* Public */,
      80,    1,  379,    2, 0x0a /* Public */,
      81,    0,  382,    2, 0x0a /* Public */,
      82,    1,  383,    2, 0x0a /* Public */,
      83,    0,  386,    2, 0x0a /* Public */,
      84,    0,  387,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, QMetaType::QByteArray,    2,    2,    2,
    QMetaType::Void, QMetaType::Double,    6,

 // slots: parameters
    QMetaType::UInt,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::UShort, QMetaType::Short, QMetaType::Bool, QMetaType::Bool,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QString,    9,
    0x80000000 | 19, QMetaType::QString, QMetaType::Bool,   20,   21,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 40,    2,
    QMetaType::Void, 0x80000000 | 42,   43,
    QMetaType::Void, 0x80000000 | 42,   45,
    QMetaType::Void, QMetaType::QString,   47,
    QMetaType::Void, QMetaType::QString,   49,
    QMetaType::Void, QMetaType::QString,   51,
    QMetaType::Void, 0x80000000 | 53,   54,
    QMetaType::Void, 0x80000000 | 56,   57,
    QMetaType::Void, QMetaType::QString,   51,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 42,   45,
    QMetaType::Bool,
    QMetaType::Bool,
    0x80000000 | 64, 0x80000000 | 65, 0x80000000 | 65, 0x80000000 | 65,   66,   67,   68,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,   70,   71,
    QMetaType::Void, 0x80000000 | 3,   70,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4, QMetaType::QByteArray,   70,   71,   74,
    QMetaType::Void, QMetaType::QString,   76,
    QMetaType::Void, QMetaType::QString,   78,
    QMetaType::Void, QMetaType::QString,   78,
    QMetaType::Void, QMetaType::QString,   78,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FTR_CTR3SpeedCtl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FTR_CTR3SpeedCtl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ServerRecved((*reinterpret_cast< qintptr(*)>(_a[1])),(*reinterpret_cast< QTcpSocket*(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 1: _t->toCalcCapacitySignal((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: { quint32 _r = _t->CheckRTLogSize();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->setRtlogFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->SetToPushInWorkSlot(); break;
        case 5: _t->CreateMapSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< qint16(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 6: _t->loadRount((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: { StationInfo_t _r = _t->GetActionBaseStationName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< StationInfo_t*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->on_readoutputSlot(); break;
        case 9: _t->CreateStreamlitAppSlot(); break;
        case 10: _t->StartStreamlitUISlot(); break;
        case 11: _t->StopStreamlitUISlot(); break;
        case 12: _t->get_ip_addr(); break;
        case 13: _t->getCartModel(); break;
        case 14: _t->getHostName(); break;
        case 15: _t->on_frpServerReadOutputSlot(); break;
        case 16: _t->on_GetSSIDReadOutputSlot(); break;
        case 17: _t->getSSIDScript(); break;
        case 18: _t->startFRPServer(); break;
        case 19: _t->killFRPServer(); break;
        case 20: _t->Time2LoopSlot(); break;
        case 21: _t->Timer2SendDataSlot(); break;
        case 22: _t->PauseToGoTimerSlot(); break;
        case 23: _t->RecoverOATimerSlot(); break;
        case 24: _t->ReadUARTSlot(); break;
        case 25: _t->UpdateVelocitySlot((*reinterpret_cast< MotorCtrlInfo(*)>(_a[1]))); break;
        case 26: _t->CartStateSyncSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 27: _t->WriteMainPipeSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 28: _t->WriteInfoToVisionPipeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->fileChangedSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 30: _t->WriteOutPipeSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->UpdateVTKInfoSlot((*reinterpret_cast< VTKInfo_t(*)>(_a[1]))); break;
        case 32: _t->UpdateVTPInfoSlot((*reinterpret_cast< VTPInfo_t(*)>(_a[1]))); break;
        case 33: _t->UpdatePipeInputSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 34: _t->PNGButtonToggleSlot(); break;
        case 35: _t->SettingOAToggleSlot((*reinterpret_cast< CartState_e(*)>(_a[1]))); break;
        case 36: { bool _r = _t->SettingOAGlobalBaseJsonSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 37: { bool _r = _t->WriteWheelDiamToBaseJsonSlot();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 38: { ArcInfo_t _r = _t->CalcArcInfoSlot((*reinterpret_cast< PointAxis_t(*)>(_a[1])),(*reinterpret_cast< PointAxis_t(*)>(_a[2])),(*reinterpret_cast< PointAxis_t(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< ArcInfo_t*>(_a[0]) = std::move(_r); }  break;
        case 39: _t->ClientConnected((*reinterpret_cast< qintptr(*)>(_a[1])),(*reinterpret_cast< QTcpSocket*(*)>(_a[2]))); break;
        case 40: _t->ClientDisconnected((*reinterpret_cast< qintptr(*)>(_a[1]))); break;
        case 41: _t->ServerRecvedSlot((*reinterpret_cast< qintptr(*)>(_a[1])),(*reinterpret_cast< QTcpSocket*(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 42: _t->tcpSocketReadSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 43: _t->tcpSocketSendMessageSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 44: _t->tcpSocketSendMessageToDeviceSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 45: _t->updSendMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 46: _t->lowPowerToShutDownSystemSlot(); break;
        case 47: _t->calcCapacitySlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 48: _t->ResetWIFISlot(); break;
        case 49: _t->getVisionVersionInfoSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 39:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        case 41:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QTcpSocket* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FTR_CTR3SpeedCtl::*)(qintptr , QTcpSocket * , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FTR_CTR3SpeedCtl::ServerRecved)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FTR_CTR3SpeedCtl::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FTR_CTR3SpeedCtl::toCalcCapacitySignal)) {
                *result = 1;
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
        if (_id < 50)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 50)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 50;
    }
    return _id;
}

// SIGNAL 0
void FTR_CTR3SpeedCtl::ServerRecved(qintptr _t1, QTcpSocket * _t2, const QByteArray & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FTR_CTR3SpeedCtl::toCalcCapacitySignal(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
