/****************************************************************************
** Meta object code from reading C++ file 'switch.h'
**
** Created: Sat Jun 16 16:42:21 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "switch.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'switch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Switch[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      22,    7,    7,    7, 0x08,
      40,    7,    7,    7, 0x08,
      56,    7,    7,    7, 0x08,
      74,    7,    7,    7, 0x08,
      92,    7,    7,    7, 0x08,
     113,    7,    7,    7, 0x08,
     124,    7,    7,    7, 0x08,
     139,    7,    7,    7, 0x08,
     154,    7,    7,    7, 0x08,
     170,    7,    7,    7, 0x08,
     184,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Switch[] = {
    "Switch\0\0openSession()\0connectToClient()\0"
    "addClientName()\0clientConnected()\0"
    "clientEncrypted()\0clientDisconnected()\0"
    "readMode()\0sendToClient()\0chooseClient()\0"
    "showDatagrams()\0switchPorts()\0"
    "quitSession()\0"
};

const QMetaObject Switch::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Switch,
      qt_meta_data_Switch, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Switch::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Switch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Switch::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Switch))
        return static_cast<void*>(const_cast< Switch*>(this));
    return QDialog::qt_metacast(_clname);
}

int Switch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openSession(); break;
        case 1: connectToClient(); break;
        case 2: addClientName(); break;
        case 3: clientConnected(); break;
        case 4: clientEncrypted(); break;
        case 5: clientDisconnected(); break;
        case 6: readMode(); break;
        case 7: sendToClient(); break;
        case 8: chooseClient(); break;
        case 9: showDatagrams(); break;
        case 10: switchPorts(); break;
        case 11: quitSession(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
