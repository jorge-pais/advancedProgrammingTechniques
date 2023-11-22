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
    QByteArrayData data[19];
    char stringdata0[237];
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
QT_MOC_LITERAL(5, 33, 12), // "attackSignal"
QT_MOC_LITERAL(6, 46, 22), // "std::shared_ptr<Enemy>"
QT_MOC_LITERAL(7, 69, 5), // "enemy"
QT_MOC_LITERAL(8, 75, 22), // "poisonLevelUpdatedSlot"
QT_MOC_LITERAL(9, 98, 5), // "value"
QT_MOC_LITERAL(10, 104, 19), // "positionChangedSlot"
QT_MOC_LITERAL(11, 124, 28), // "protagonistHealthChangedSlot"
QT_MOC_LITERAL(12, 153, 1), // "h"
QT_MOC_LITERAL(13, 155, 28), // "protagonistEnergyChangedSlot"
QT_MOC_LITERAL(14, 184, 1), // "e"
QT_MOC_LITERAL(15, 186, 13), // "enemyDeadSlot"
QT_MOC_LITERAL(16, 200, 19), // "mainWindowEventSlot"
QT_MOC_LITERAL(17, 220, 10), // "QKeyEvent*"
QT_MOC_LITERAL(18, 231, 5) // "event"

    },
    "WorldView\0playerMovedSignal\0\0x\0y\0"
    "attackSignal\0std::shared_ptr<Enemy>\0"
    "enemy\0poisonLevelUpdatedSlot\0value\0"
    "positionChangedSlot\0protagonistHealthChangedSlot\0"
    "h\0protagonistEnergyChangedSlot\0e\0"
    "enemyDeadSlot\0mainWindowEventSlot\0"
    "QKeyEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WorldView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       5,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   62,    2, 0x0a /* Public */,
      10,    2,   65,    2, 0x0a /* Public */,
      11,    1,   70,    2, 0x0a /* Public */,
      13,    1,   73,    2, 0x0a /* Public */,
      15,    0,   76,    2, 0x0a /* Public */,
      16,    1,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,

       0        // eod
};

void WorldView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WorldView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->playerMovedSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->attackSignal((*reinterpret_cast< std::shared_ptr<Enemy>(*)>(_a[1]))); break;
        case 2: _t->poisonLevelUpdatedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->positionChangedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->protagonistHealthChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->protagonistEnergyChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->enemyDeadSlot(); break;
        case 7: _t->mainWindowEventSlot((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
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
        {
            using _t = void (WorldView::*)(std::shared_ptr<Enemy> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WorldView::attackSignal)) {
                *result = 1;
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
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void WorldView::playerMovedSignal(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WorldView::attackSignal(std::shared_ptr<Enemy> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
