#include "qsslserver.h"

QSslServer::QSslServer(QObject * parent)
	: QTcpServer(parent)
{
}

void QSslServer::incomingConnection(int socketDescriptor)
{
    QSslSocket *serverSocket = new QSslSocket;
    if (serverSocket->setSocketDescriptor(socketDescriptor)) {
        connect(serverSocket, SIGNAL(encrypted()), this, SLOT(ready()));
        serverSocket->startServerEncryption();
    } else {
        delete serverSocket;
    }
}

