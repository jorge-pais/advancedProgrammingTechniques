/****************************************************************************
** Meta object code from reading C++ file 'worldview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "worldview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'worldview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WorldView_t {
    QByteArrayData data[13];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WorldView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WorldView_t qt_meta_stringdata_WorldView = {
    {
QT_MOC_LITERAL(0, 0, 9), // "WorldView"
QT_MOC_LITERAL(1, 10, 17), // "playerMovedSignal"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 1), // "x"
QT_MOC_LITERAL(4, 31, 1), // "y"
QT_MOC_LITERAL(5, 33, 22), // "poisonLevelUpdatedSlot"
QT_MOC_LITERAL(6, 56, 5), // "value"
QT_MOC_LITERAL(7, 62, 19), // "positionChangedSlot"
QT_MOC_LITERAL(8, 82, 28), // "protagonistHealthChangedSlot"
QT_MOC_LITERAL(9, 111, 1), // "h"
QT_MOC_LITERAL(10, 113, 28), // "protagonistEnergyChangedSlot"
QT_MOC_LITERAL(11, 142, 1), // "e"
QT_MOC_LITERAL(12, 144, 13) // "enemyDeadSlot"

    },
    "WorldView\0playerMovedSignal\0\0x\0y\0"
    "poisonLevelUpdatedSlot\0value\0"
    "positionChangedSlot\0protagonistHealthChangedSlot\0"
    "h\0protagonistEnergyChangedSlot\0e\0"
    "enemyDeadSlot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WorldView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   49,    2, 0x0a /* Public */,
       7,    2,   52,    2, 0x0a /* Public */,
       8,    1,   57,    2, 0x0a /* Public */,
      10,    1,   60,    2, 0x0a /* Public */,
      12,    0,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,

       0        // eod
};

void WorldView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WorldView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->playerMovedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->poisonLevelUpdatedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->positionChangedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->protagonistHealthChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->protagonistEnergyChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->enemyDeadSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WorldView::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WorldView::playerMovedSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WorldView::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WorldView.data,
    qt_meta_data_WorldView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WorldView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WorldView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WorldView.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WorldView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void WorldView::playerMovedSignal(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
