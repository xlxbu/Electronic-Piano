/****************************************************************************
** Meta object code from reading C++ file 'communicate.h'
**
** Created: Fri Dec 13 20:42:37 2013
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "communicate.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communicate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Communicate[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      22,   12,   12,   12, 0x08,
      31,   12,   12,   12, 0x08,
      49,   12,   12,   12, 0x08,
      62,   12,   12,   12, 0x08,
      69,   12,   12,   12, 0x08,
      77,   12,   12,   12, 0x08,
      91,   12,   12,   12, 0x08,
     103,   12,   12,   12, 0x08,
     115,   12,   12,   12, 0x08,
     127,   12,   12,   12, 0x08,
     139,   12,   12,   12, 0x08,
     151,   12,   12,   12, 0x08,
     163,   12,   12,   12, 0x08,
     175,   12,   12,   12, 0x08,
     193,   12,   12,   12, 0x08,
     211,   12,   12,   12, 0x08,
     229,   12,   12,   12, 0x08,
     247,   12,   12,   12, 0x08,
     265,   12,   12,   12, 0x08,
     274,   12,   12,   12, 0x08,
     285,   12,   12,   12, 0x08,
     300,   12,   12,   12, 0x08,
     314,   12,   12,   12, 0x08,
     327,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Communicate[] = {
    "Communicate\0\0OnSave()\0OnMode()\0"
    "SearchBluetooth()\0SearchFile()\0Play()\0"
    "Pause()\0Connect_dev()\0Keyboard1()\0"
    "Keyboard2()\0Keyboard3()\0Keyboard4()\0"
    "Keyboard5()\0Keyboard6()\0Keyboard7()\0"
    "Keyboard_black1()\0Keyboard_black2()\0"
    "Keyboard_black3()\0Keyboard_black4()\0"
    "Keyboard_black5()\0Volume()\0Nextsong()\0"
    "Previoussong()\0setTitle(int)\0clearItems()\0"
    "clearFiles()\0"
};

const QMetaObject Communicate::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Communicate,
      qt_meta_data_Communicate, 0 }
};

const QMetaObject *Communicate::metaObject() const
{
    return &staticMetaObject;
}

void *Communicate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Communicate))
        return static_cast<void*>(const_cast< Communicate*>(this));
    return QWidget::qt_metacast(_clname);
}

int Communicate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnSave(); break;
        case 1: OnMode(); break;
        case 2: SearchBluetooth(); break;
        case 3: SearchFile(); break;
        case 4: Play(); break;
        case 5: Pause(); break;
        case 6: Connect_dev(); break;
        case 7: Keyboard1(); break;
        case 8: Keyboard2(); break;
        case 9: Keyboard3(); break;
        case 10: Keyboard4(); break;
        case 11: Keyboard5(); break;
        case 12: Keyboard6(); break;
        case 13: Keyboard7(); break;
        case 14: Keyboard_black1(); break;
        case 15: Keyboard_black2(); break;
        case 16: Keyboard_black3(); break;
        case 17: Keyboard_black4(); break;
        case 18: Keyboard_black5(); break;
        case 19: Volume(); break;
        case 20: Nextsong(); break;
        case 21: Previoussong(); break;
        case 22: setTitle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: clearItems(); break;
        case 24: clearFiles(); break;
        default: ;
        }
        _id -= 25;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
