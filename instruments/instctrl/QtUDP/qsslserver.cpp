#include "qsslserver.h"

QSslServer::QSslServer(QObject * parent)
	: QTcpServer(parent)
{
}

void QSslServer::incomingConnection(int socketDescriptor)
{
	QSslSocket *socket = new QSslSocket;
	if (socket->setSocketDescriptor(socketDescriptor)) {
		pendingConnections_.append(socket);

		QFile keyFile("certs/server.key");
		QFile clientFile("certs/client.crt");
		QFile serverFile("certs/server.crt");
		keyFile.open(QIODevice::ReadOnly);
		clientFile.open(QIODevice::ReadOnly);
		serverFile.open(QIODevice::ReadOnly);

		QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, QByteArray("server"));

		if (key.isNull()) {
			qWarning("key is null");
			socket->disconnectFromHost();
			return;
		}

		QSslCertificate clientCert(&clientFile);
		QSslCertificate serverCert(&serverFile);

		connectDebuggingMessages(socket);

		socket->setPrivateKey(key);
		socket->setLocalCertificate(serverCert);

		QSslError error(QSslError::SelfSignedCertificate, clientCert);
		QList<QSslError> expectedSslErrors;
		expectedSslErrors.append(QSslError::SelfSignedCertificate);

		socket->ignoreSslErrors(expectedSslErrors);
		socket->startServerEncryption();
	} else {
		delete socket;
	}
}

QSslSocket *QSslServer::nextPendingConnection()
{
	if (pendingConnections_.isEmpty()) {
		return 0;
	}
	return pendingConnections_.takeFirst();
}

void QSslServer::connectDebuggingMessages(QSslSocket * serverSocket)
{
	connect(serverSocket, SIGNAL(encrypted()), this, SLOT(slot_encrypted()));
	connect(serverSocket, SIGNAL(encryptedBytesWritten(qint64)), this, SLOT(slot_encryptedBytesWritten(qint64)));
	connect(serverSocket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(slot_modeChanged(QSslSocket::SslMode)));
	connect(serverSocket, SIGNAL(peerVerifyError(const QSslError &)), this, SLOT(slot_peerVerifyError (const QSslError &)));
	connect(serverSocket, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(slot_sslErrors(const QList<QSslError> &)));
	connect(serverSocket, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
	connect(serverSocket, SIGNAL(connected()), this, SLOT(slot_connected()));
	connect(serverSocket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
	connect(serverSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slot_error(QAbstractSocket::SocketError)));
	connect(serverSocket, SIGNAL(hostFound()), this, SLOT(slot_hostFound()));
	connect(serverSocket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
			this, SLOT(slot_proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
	connect(serverSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));
}

void QSslServer::slot_encrypted ()
{
   qDebug("QMyServer::slot_encrypted");
}

void QSslServer::slot_encryptedBytesWritten (qint64 written)
{
   qDebug("QMyServer::slot_encryptedBytesWritten(%ld)", (long) written);
}

void QSslServer::slot_modeChanged (QSslSocket::SslMode mode)
{
   qDebug("QMyServer::slot_modeChanged(%d)", mode);
}

void QSslServer::slot_peerVerifyError (const QSslError & err)
{
   qDebug() << "QMyServer::slot_peerVerifyError: " << err.errorString();
}

void QSslServer::slot_sslErrors (const QList<QSslError> &)
{
   qDebug("QMyServer::slot_sslErrors");
}

void QSslServer::slot_readyRead ()
{
   qDebug("QMyServer::slot_readyRead");
}

void QSslServer::slot_connected ()
{
   qDebug("QMyServer::slot_connected");
}

void QSslServer::slot_disconnected ()
{
   qDebug("QMyServer::slot_disconnected");
}

void QSslServer::slot_error (QAbstractSocket::SocketError err)
{
   qDebug() << "QMyServer::slot_error(" << err << ")";
}

void QSslServer::slot_hostFound ()
{
   qDebug("QMyServer::slot_hostFound");
}

void QSslServer::slot_proxyAuthenticationRequired (const QNetworkProxy &, QAuthenticator *)
{
   qDebug("QMyServer::slot_proxyAuthenticationRequired");
}

void QSslServer::slot_stateChanged (QAbstractSocket::SocketState state)
{
   qDebug() << "QMyServer::slot_stateChanged(" << state << ")";
}
