/****************************************************************************
** Meta object code from reading C++ file 'ParticleEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/ParticleEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ParticleEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ParticleEditor_t {
    QByteArrayData data[20];
    char stringdata[233];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ParticleEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ParticleEditor_t qt_meta_stringdata_ParticleEditor = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 7),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 8),
QT_MOC_LITERAL(4, 33, 8),
QT_MOC_LITERAL(5, 42, 10),
QT_MOC_LITERAL(6, 53, 8),
QT_MOC_LITERAL(7, 62, 13),
QT_MOC_LITERAL(8, 76, 13),
QT_MOC_LITERAL(9, 90, 17),
QT_MOC_LITERAL(10, 108, 13),
QT_MOC_LITERAL(11, 122, 13),
QT_MOC_LITERAL(12, 136, 15),
QT_MOC_LITERAL(13, 152, 16),
QT_MOC_LITERAL(14, 169, 16),
QT_MOC_LITERAL(15, 186, 3),
QT_MOC_LITERAL(16, 190, 16),
QT_MOC_LITERAL(17, 207, 3),
QT_MOC_LITERAL(18, 211, 16),
QT_MOC_LITERAL(19, 228, 3)
    },
    "ParticleEditor\0MenuNew\0\0MenuOpen\0"
    "MenuSave\0ManuSaveAs\0MenuExit\0MenuViewStats\0"
    "MenuHelpAbout\0MenuCreateEmitter\0"
    "DeleteEmitter\0RenameEmitter\0EmitterSelected\0"
    "QListWidgetItem*\0PositionXChanged\0p_x\0"
    "PositionYChanged\0p_y\0PositionZChanged\0"
    "p_z\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParticleEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08,
       3,    0,   85,    2, 0x08,
       4,    0,   86,    2, 0x08,
       5,    0,   87,    2, 0x08,
       6,    0,   88,    2, 0x08,
       7,    0,   89,    2, 0x08,
       8,    0,   90,    2, 0x08,
       9,    0,   91,    2, 0x08,
      10,    0,   92,    2, 0x08,
      11,    0,   93,    2, 0x08,
      12,    1,   94,    2, 0x08,
      14,    1,   97,    2, 0x08,
      16,    1,  100,    2, 0x08,
      18,    1,  103,    2, 0x08,

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
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void, QMetaType::Double,   15,
    QMetaType::Void, QMetaType::Double,   17,
    QMetaType::Void, QMetaType::Double,   19,

       0        // eod
};

void ParticleEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ParticleEditor *_t = static_cast<ParticleEditor *>(_o);
        switch (_id) {
        case 0: _t->MenuNew(); break;
        case 1: _t->MenuOpen(); break;
        case 2: _t->MenuSave(); break;
        case 3: _t->ManuSaveAs(); break;
        case 4: _t->MenuExit(); break;
        case 5: _t->MenuViewStats(); break;
        case 6: _t->MenuHelpAbout(); break;
        case 7: _t->MenuCreateEmitter(); break;
        case 8: _t->DeleteEmitter(); break;
        case 9: _t->RenameEmitter(); break;
        case 10: _t->EmitterSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 11: _t->PositionXChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->PositionYChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->PositionZChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ParticleEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ParticleEditor.data,
      qt_meta_data_ParticleEditor,  qt_static_metacall, 0, 0}
};


const QMetaObject *ParticleEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParticleEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ParticleEditor.stringdata))
        return static_cast<void*>(const_cast< ParticleEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ParticleEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
