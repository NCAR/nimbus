/****************************************************************************
** Meta object code from reading C++ file 'sslserver.h'
**
** Created: Sat Jun 9 03:16:59 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sslserver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sslserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SslServer[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      25,   10,   10,   10, 0x08,
      43,   10,   10,   10, 0x08,
      54,   10,   10,   10, 0x08,
      68,   10,   10,   10, 0x08,
      79,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SslServer[] = {
    "SslServer\0\0openSession()\0connectToClient()\0"
    "sendMode()\0sendMessage()\0readMode()\0"
    "switchPorts()\0"
};

const QMetaObject SslServer::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SslServer,
      qt_meta_data_SslServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SslServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SslServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SslServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SslServer))
        return static_cast<void*>(const_cast< SslServer*>(this));
    return QDialog::qt_metacast(_clname);
}

int SslServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openSession(); break;
        case 1: connectToClient(); break;
        case 2: sendMode(); break;
        case 3: sendMessage(); break;
        case 4: readMode(); break;
        case 5: switchPorts(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
