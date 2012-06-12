/****************************************************************************
** Meta object code from reading C++ file 'sslclient.h'
**
** Created: Fri Jun 8 13:20:50 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sslclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sslclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SslClient[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      29,   10,   10,   10, 0x08,
      40,   10,   10,   10, 0x08,
      69,   57,   10,   10, 0x08,
     119,  112,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SslClient[] = {
    "SslClient\0\0connectToServer()\0sendMode()\0"
    "switchHostMode()\0socketError\0"
    "displayError(QAbstractSocket::SocketError)\0"
    "errors\0displayError(QList<QSslError>)\0"
};

const QMetaObject SslClient::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SslClient,
      qt_meta_data_SslClient, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SslClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SslClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SslClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SslClient))
        return static_cast<void*>(const_cast< SslClient*>(this));
    return QDialog::qt_metacast(_clname);
}

int SslClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connectToServer(); break;
        case 1: sendMode(); break;
        case 2: switchHostMode(); break;
        case 3: displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 4: displayError((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
