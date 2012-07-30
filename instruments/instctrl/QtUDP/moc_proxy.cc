/****************************************************************************
** Meta object code from reading C++ file 'proxy.h'
**
** Created: Sun Jun 17 11:07:25 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "proxy.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'proxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Proxy[] = {

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
       7,    6,    6,    6, 0x08,
      25,    6,    6,    6, 0x08,
      43,    6,    6,    6, 0x08,
      61,    6,    6,    6, 0x08,
      82,    6,    6,    6, 0x08,
      93,    6,    6,    6, 0x08,
     104,    6,    6,    6, 0x08,
     119,    6,    6,    6, 0x08,
     136,    6,    6,    6, 0x08,
     162,  150,    6,    6, 0x08,
     212,  205,    6,    6, 0x08,
     243,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Proxy[] = {
    "Proxy\0\0connectToServer()\0clientConnected()\0"
    "clientEncrypted()\0clientDisconnected()\0"
    "readMode()\0sendMode()\0sendToServer()\0"
    "switchHostMode()\0quitSession()\0"
    "socketError\0displayError(QAbstractSocket::SocketError)\0"
    "errors\0displayError(QList<QSslError>)\0"
    "slot_stateChanged(QAbstractSocket::SocketState)\0"
};

const QMetaObject Proxy::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Proxy,
      qt_meta_data_Proxy, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Proxy::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Proxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Proxy::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Proxy))
        return static_cast<void*>(const_cast< Proxy*>(this));
    return QDialog::qt_metacast(_clname);
}

int Proxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectToServer(); break;
        case 1: clientConnected(); break;
        case 2: clientEncrypted(); break;
        case 3: clientDisconnected(); break;
        case 4: readMode(); break;
        case 5: sendMode(); break;
        case 6: sendToServer(); break;
        case 7: switchHostMode(); break;
        case 8: quitSession(); break;
        case 9: displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 10: displayError((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 11: slot_stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
