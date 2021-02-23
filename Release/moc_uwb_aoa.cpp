/****************************************************************************
** Meta object code from reading C++ file 'uwb_aoa.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../uwb_aoa.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uwb_aoa.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UWB_AOA_t {
    QByteArrayData data[9];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UWB_AOA_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UWB_AOA_t qt_meta_stringdata_UWB_AOA = {
    {
QT_MOC_LITERAL(0, 0, 7), // "UWB_AOA"
QT_MOC_LITERAL(1, 8, 16), // "UpdateInfoSignal"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 9), // "UWBInfo_t"
QT_MOC_LITERAL(4, 36, 4), // "info"
QT_MOC_LITERAL(5, 41, 23), // "startDataAnalysisSignal"
QT_MOC_LITERAL(6, 65, 4), // "data"
QT_MOC_LITERAL(7, 70, 12), // "ReadUARTSlot"
QT_MOC_LITERAL(8, 83, 16) // "AnalysisDealSlot"

    },
    "UWB_AOA\0UpdateInfoSignal\0\0UWBInfo_t\0"
    "info\0startDataAnalysisSignal\0data\0"
    "ReadUARTSlot\0AnalysisDealSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UWB_AOA[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   40,    2, 0x08 /* Private */,
       8,    1,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QByteArray,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,

       0        // eod
};

void UWB_AOA::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UWB_AOA *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateInfoSignal((*reinterpret_cast< UWBInfo_t(*)>(_a[1]))); break;
        case 1: _t->startDataAnalysisSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->ReadUARTSlot(); break;
        case 3: _t->AnalysisDealSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UWB_AOA::*)(UWBInfo_t );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UWB_AOA::UpdateInfoSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UWB_AOA::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UWB_AOA::startDataAnalysisSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UWB_AOA::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_UWB_AOA.data,
    qt_meta_data_UWB_AOA,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UWB_AOA::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UWB_AOA::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UWB_AOA.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int UWB_AOA::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void UWB_AOA::UpdateInfoSignal(UWBInfo_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UWB_AOA::startDataAnalysisSignal(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
