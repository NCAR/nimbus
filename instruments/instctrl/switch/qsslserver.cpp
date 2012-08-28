#include "qsslserver.h"

static QString certDir("/home/local/raf/instruments/instctrl/certs");

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
		QFile shirazClientFile("certs/shiraz_client.crt");
		QFile tikalClientFile("certs/tikal_client.crt");
		QFile dropletClientFile("certs/droplet_client.crt");

		QFile eolServerFile(certDir + QString("/eol-rt-data_server.crt"));

		keyFile.open(QIODevice::ReadOnly);
		clientFile.open(QIODevice::ReadOnly);
		shirazClientFile.open(QIODevice::ReadOnly);
		tikalClientFile.open(QIODevice::ReadOnly);
		dropletClientFile.open(QIODevice::ReadOnly);

		eolServerFile.open(QIODevice::ReadOnly);

		QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, QByteArray("server"));

		if (key.isNull()) {
			qWarning("key is null");
			socket->disconnectFromHost();
			return;
		}

		QSslCertificate clientCert(&clientFile);
		QSslCertificate shirazClientCert(&shirazClientFile);
		QSslCertificate tikalClientCert(&tikalClientFile);
		QSslCertificate dropletClientCert(&dropletClientFile);

		QSslCertificate eolServerCert(&eolServerFile);

		QList<QSslCertificate> clientCertificates;
		clientCertificates.append(clientCert);
		clientCertificates.append(shirazClientCert);
		clientCertificates.append(tikalClientCert);
		clientCertificates.append(dropletClientCert);

		QList<QSslCertificate> serverCertificates;
		serverCertificates.append(eolServerCert);

		// Can uncomment this line to see debugging messages during
		// certificate verification process
		connectDebuggingMessages(socket);

		socket->setPrivateKey(key);
		socket->setCaCertificates(clientCertificates);
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
		QSslError shirazError(QSslError::SelfSignedCertificate, shirazClientCert);
		QSslError tikalError(QSslError::SelfSignedCertificate, tikalClientCert);
		QSslError dropletError(QSslError::SelfSignedCertificate, dropletClientCert);

		QList<QSslError> expectedSslErrors;
		expectedSslErrors.append(error);
		expectedSslErrors.append(shirazError);
		expectedSslErrors.append(tikalError);
		expectedSslErrors.append(dropletError);

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
