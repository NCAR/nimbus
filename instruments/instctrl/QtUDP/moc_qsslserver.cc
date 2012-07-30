/****************************************************************************
** Meta object code from reading C++ file 'qsslserver.h'
**
** Created: Sun Jun 17 05:08:59 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qsslserver.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qsslserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QSslServer[] = {

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
      12,   11,   11,   11, 0x08,
      37,   29,   11,   11, 0x08,
      77,   72,   11,   11, 0x08,
     121,  115,   11,   11, 0x08,
     160,  153,   11,   11, 0x08,
     193,   11,   11,   11, 0x08,
     210,   11,   11,   11, 0x08,
     230,   11,   11,   11, 0x08,
     271,   11,   11,   11, 0x08,
     290,  288,   11,   11, 0x08,
     354,   11,   11,   11, 0x08,
     402,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QSslServer[] = {
    "QSslServer\0\0slot_encrypted()\0written\0"
    "slot_encryptedBytesWritten(qint64)\0"
    "mode\0slot_modeChanged(QSslSocket::SslMode)\0"
    "error\0slot_peerVerifyError(QSslError)\0"
    "errors\0slot_sslErrors(QList<QSslError>)\0"
    "slot_connected()\0slot_disconnected()\0"
    "slot_error(QAbstractSocket::SocketError)\0"
    "slot_hostFound()\0,\0"
    "slot_proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)\0"
    "slot_stateChanged(QAbstractSocket::SocketState)\0"
    "slot_readyRead()\0"
};

const QMetaObject QSslServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_QSslServer,
      qt_meta_data_QSslServer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QSslServer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QSslServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QSslServer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QSslServer))
        return static_cast<void*>(const_cast< QSslServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int QSslServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slot_encrypted(); break;
        case 1: slot_encryptedBytesWritten((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 2: slot_modeChanged((*reinterpret_cast< QSslSocket::SslMode(*)>(_a[1]))); break;
        case 3: slot_peerVerifyError((*reinterpret_cast< const QSslError(*)>(_a[1]))); break;
        case 4: slot_sslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 5: slot_connected(); break;
        case 6: slot_disconnected(); break;
        case 7: slot_error((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 8: slot_hostFound(); break;
        case 9: slot_proxyAuthenticationRequired((*reinterpret_cast< const QNetworkProxy(*)>(_a[1])),(*reinterpret_cast< QAuthenticator*(*)>(_a[2]))); break;
        case 10: slot_stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 11: slot_readyRead(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
