/****************************************************************************
** Meta object code from reading C++ file 'Treenity.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/Treenity.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Treenity.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Treenity_t {
    QByteArrayData data[11];
    char stringdata[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Treenity_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Treenity_t qt_meta_stringdata_Treenity = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 3),
QT_MOC_LITERAL(2, 13, 0),
QT_MOC_LITERAL(3, 14, 11),
QT_MOC_LITERAL(4, 26, 4),
QT_MOC_LITERAL(5, 31, 6),
QT_MOC_LITERAL(6, 38, 12),
QT_MOC_LITERAL(7, 51, 13),
QT_MOC_LITERAL(8, 65, 12),
QT_MOC_LITERAL(9, 78, 20),
QT_MOC_LITERAL(10, 99, 13)
    },
    "Treenity\0New\0\0OpenProject\0Save\0SaveAs\0"
    "CreateEntity\0DestroyEntity\0RenameEntity\0"
    "OutlinerSelectEntity\0AddRenderable\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Treenity[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08,
       3,    0,   60,    2, 0x08,
       4,    0,   61,    2, 0x08,
       5,    0,   62,    2, 0x08,
       6,    0,   63,    2, 0x08,
       7,    0,   64,    2, 0x08,
       8,    0,   65,    2, 0x08,
       9,    0,   66,    2, 0x08,
      10,    0,   67,    2, 0x08,

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

       0        // eod
};

void Treenity::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Treenity *_t = static_cast<Treenity *>(_o);
        switch (_id) {
        case 0: _t->New(); break;
        case 1: _t->OpenProject(); break;
        case 2: _t->Save(); break;
        case 3: _t->SaveAs(); break;
        case 4: _t->CreateEntity(); break;
        case 5: _t->DestroyEntity(); break;
        case 6: _t->RenameEntity(); break;
        case 7: _t->OutlinerSelectEntity(); break;
        case 8: _t->AddRenderable(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Treenity::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Treenity.data,
      qt_meta_data_Treenity,  qt_static_metacall, 0, 0}
};


const QMetaObject *Treenity::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Treenity::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Treenity.stringdata))
        return static_cast<void*>(const_cast< Treenity*>(this));
    if (!strcmp(_clname, "EditorInterface"))
        return static_cast< EditorInterface*>(const_cast< Treenity*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Treenity::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE