/****************************************************************************
** Meta object code from reading C++ file 'clock_time.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../clock_Time/clock_time.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clock_time.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_clock_time_t {
    uint offsetsAndSizes[6];
    char stringdata0[11];
    char stringdata1[8];
    char stringdata2[1];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_clock_time_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_clock_time_t qt_meta_stringdata_clock_time = {
    {
        QT_MOC_LITERAL(0, 10),  // "clock_time"
        QT_MOC_LITERAL(11, 7),  // "setTime"
        QT_MOC_LITERAL(19, 0)   // ""
    },
    "clock_time",
    "setTime",
    ""
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_clock_time[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   20,    2, 0x08,    1 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject clock_time::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_clock_time.offsetsAndSizes,
    qt_meta_data_clock_time,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_clock_time_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<clock_time, std::true_type>,
        // method 'setTime'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void clock_time::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<clock_time *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->setTime(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *clock_time::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *clock_time::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_clock_time.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int clock_time::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
