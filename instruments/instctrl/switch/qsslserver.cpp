#include "qsslserver.h"

QSslServer::QSslServer(QObject * parent)
	: QTcpServer(parent)
{
}

void QSslServer::incomingConnection(int socketDescriptor)
{
qDebug("QMyServer::incomingConnection");
	QSslSocket *socket = new QSslSocket;
	if (socket->setSocketDescriptor(socketDescriptor)) {
		pendingConnections_.append(socket);

		QFile keyFile("certs/server.key");
		QFile clientFile("certs/client.crt");
		//QFile serverFile("certs/server.crt");
		//QFile tikalClientFile("certs/tikal_client.crt");
		//QFile tikalServerFile("certs/tikal_server.crt");
		QFile shirazClientFile("certs/shiraz_client.crt");
		QFile newshirazClientFile("certs/newshiraz.crt");
		//QFile shirazServerFile("certs/shiraz_server.crt");
		//QFile dropletClientFile("certs/droplet_client.crt");
		//QFile dropletServerFile("certs/droplet_server.crt");
		//QFile sloopClientFile("certs/sloop_client.crt");
		//QFile sloopServerFile("certs/sloop_server.crt");
		//QFile multiHostClientFile("certs/san_client.crt");
		//QFile multiHostServerFile("certs/san_server.crt");

		QFile eolServerFile("certs/eol-rt-data_server.crt");

		keyFile.open(QIODevice::ReadOnly);
		clientFile.open(QIODevice::ReadOnly);
		//serverFile.open(QIODevice::ReadOnly);
		//tikalClientFile.open(QIODevice::ReadOnly);
		//tikalServerFile.open(QIODevice::ReadOnly);
		shirazClientFile.open(QIODevice::ReadOnly);
		newshirazClientFile.open(QIODevice::ReadOnly);
		//shirazServerFile.open(QIODevice::ReadOnly);
		//dropletClientFile.open(QIODevice::ReadOnly);
		//dropletServerFile.open(QIODevice::ReadOnly);
		//sloopClientFile.open(QIODevice::ReadOnly);
		//sloopServerFile.open(QIODevice::ReadOnly);
		//multiHostClientFile.open(QIODevice::ReadOnly);
		//multiHostServerFile.open(QIODevice::ReadOnly);

		eolServerFile.open(QIODevice::ReadOnly);

		QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, QByteArray("server"));

		if (key.isNull()) {
			qWarning("key is null");
			socket->disconnectFromHost();
			return;
		}

		QSslCertificate clientCert(&clientFile);
		//QSslCertificate serverCert(&serverFile);
		//QSslCertificate tikalClientCert(&tikalClientFile);
		//QSslCertificate tikalServerCert(&tikalServerFile);
		QSslCertificate shirazClientCert(&shirazClientFile);
		QSslCertificate newshirazClientCert(&newshirazClientFile);
		//QSslCertificate shirazServerCert(&shirazServerFile);
		//QSslCertificate dropletClientCert(&dropletClientFile);
		//QSslCertificate dropletServerCert(&dropletServerFile);
		//QSslCertificate sloopClientCert(&sloopClientFile);
		//QSslCertificate sloopServerCert(&sloopServerFile);
		//QSslCertificate multiHostClientCert(&multiHostClientFile);
		//QSslCertificate multiHostServerCert(&multiHostServerFile);

		QSslCertificate eolServerCert(&eolServerFile);

		QList<QSslCertificate> clientCertificates;
		clientCertificates.append(clientCert);
		//clientCertificates.append(tikalClientCert);
		clientCertificates.append(shirazClientCert);
		clientCertificates.append(newshirazClientCert);
		//clientCertificates.append(dropletClientCert);
		//clientCertificates.append(sloopClientCert);
		//clientCertificates.append(multiHostClientCert);

		QList<QSslCertificate> serverCertificates;
		//serverCertificates.append(serverCert);
		//serverCertificates.append(tikalServerCert);
		//serverCertificates.append(shirazServerCert);
		//serverCertificates.append(dropletServerCert);
		//serverCertificates.append(sloopServerCert);
		//serverCertificates.append(multiHostServerCert);
		serverCertificates.append(eolServerCert);

		// Can uncomment this line to see debugging messages during
		// certificate verification process
		connectDebuggingMessages(socket);

		socket->setPrivateKey(key);
		socket->setCaCertificates(clientCertificates);
		//socket->addCaCertificates(clientCertificates);
		socket->addCaCertificates(serverCertificates);
		socket->setPeerVerifyMode(QSslSocket::VerifyPeer);

		socket->setLocalCertificate(eolServerCert);

		QMultiMap<QSsl::AlternateNameEntryType, QString> alternates = socket->localCertificate().alternateSubjectNames();
		if (alternates.isEmpty()) {
			qDebug("No alternates in server certificate.");
		} else {
			qDebug() << "Subject Alternate Names for server:\n" << alternates;
		}

		QSslError error(QSslError::SelfSignedCertificate, clientCert);
		QSslError newshirazerror(QSslError::SelfSignedCertificate, newshirazClientCert);
		QSslError shirazError(QSslError::SelfSignedCertificate, shirazClientCert);
		//QSslError multiHostError(QSslError::SelfSignedCertificate, multiHostClientCert);
		//QSslError tikalError(QSslError::SelfSignedCertificate, tikalClientCert);
		//QSslError dropletError(QSslError::SelfSignedCertificate, dropletClientCert);
		//QSslError sloopError(QSslError::SelfSignedCertificate, sloopClientCert);

		QList<QSslError> expectedSslErrors;
		expectedSslErrors.append(error);
		//expectedSslErrors.append(multiHostError);
		//expectedSslErrors.append(tikalError);
		expectedSslErrors.append(shirazError);
		expectedSslErrors.append(newshirazerror);
		//expectedSslErrors.append(dropletError);
		//expectedSslErrors.append(sloopError);
		socket->startServerEncryption();
		//socket->ignoreSslErrors(expectedSslErrors);
	} else {
		delete socket;
	}
}

QSslSocket *QSslServer::nextPendingConnection()
{
qDebug("QMyServer::nextPendingConnection");
	if (pendingConnections_.isEmpty()) {
		return 0;
	}
	return pendingConnections_.takeFirst();
}

void QSslServer::connectDebuggingMessages(QSslSocket * serverSocket)
{
qDebug("QMyServer::connectDebuggingMessages");
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
   qDebug() << "QMyServer::slot_peerVerifyError:" << err;
}

void QSslServer::slot_sslErrors (const QList<QSslError> & errs)
{
   qDebug() << "QMyServer::slot_sslErrors:" << errs;
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
