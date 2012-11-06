#include "qsslserver.h"

static QString certDir("/home/tbaltzer/raf/instruments/instctrl/QtUDP/certs");

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

		QFile keyFile(certDir + QString("/server.key"));
		QFile clientFile(certDir + QString("/client.crt"));
		QFile shirazClientFile(certDir + QString("/shiraz_client.crt"));
		QFile tikalClientFile(certDir + QString("/tikal_client.crt"));
		QFile dropletClientFile(certDir + QString("/droplet_client.crt"));

		QFile eolServerFile(certDir + QString("/eol-rt-data_server.crt"));

		if (!openCertFile(keyFile)) return;
                if (!openCertFile(clientFile)) return;
		if (!openCertFile(shirazClientFile)) return;
                if (!openCertFile(tikalClientFile)) return;
                if (!openCertFile(dropletClientFile)) return;
                if (!openCertFile(eolServerFile)) return;

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
		//connectDebuggingMessages(socket);

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
   switch (err) {
      case QAbstractSocket::ConnectionRefusedError:
         qDebug("The connection was refused by the peer (or timed out).");
         break;
      case QAbstractSocket::RemoteHostClosedError:
         qDebug("The remote host closed the connection. Note that the client socket.");
         break;
      case QAbstractSocket::HostNotFoundError:
         qDebug("The host address was not found.");
         break;
      case QAbstractSocket::SocketAccessError:
         qDebug("The socket operation failed because the application lacked the required privileges.");
         break;
      case QAbstractSocket::SocketResourceError:
         qDebug("The local system ran out of resources (e.g., too many sockets).");
         break;
      case QAbstractSocket::SocketTimeoutError:
         qDebug("The socket operation timed out.");
         break;
      case QAbstractSocket::DatagramTooLargeError:
         qDebug("The datagram was larger than the operating system's limit.");
         break;
      case QAbstractSocket::NetworkError:
         qDebug("An error occurred with the network (e.g., the network cable was accidentally plugged out).");
         break;
      case QAbstractSocket::AddressInUseError:
         qDebug("The address specified to QUdpSocket::bind() is already in use and was set to be exclusive.");
         break;
      case QAbstractSocket::SocketAddressNotAvailableError:
         qDebug("The address specified to QUdpSocket::bind() does not belong to the host.");
         break;
      case QAbstractSocket::UnsupportedSocketOperationError:
         qDebug("The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).");
         break;
      case QAbstractSocket::ProxyAuthenticationRequiredError:
         qDebug("The socket is using a proxy, and the proxy requires authentication.");
         break;
      case QAbstractSocket::SslHandshakeFailedError:
         qDebug("The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)");
         break;
      case QAbstractSocket::UnfinishedSocketOperationError:
         qDebug("Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).");
         break;
      case QAbstractSocket::ProxyConnectionRefusedError:
         qDebug("Could not contact the proxy server because the connection to that server was denied.");
         break;
      case QAbstractSocket::ProxyConnectionClosedError:
         qDebug("The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)");
         break;
      case QAbstractSocket::ProxyConnectionTimeoutError:
         qDebug("The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.");
         break;
      case QAbstractSocket::ProxyNotFoundError:
         qDebug("The proxy address set with setProxy() (or the application proxy) was not found.");
         break;
      case QAbstractSocket::ProxyProtocolError:
         qDebug("The connection negotiation with the proxy server because the response from the proxy server could not be understood.");
         break;
      case QAbstractSocket::UnknownSocketError:
         qDebug("An unidentified error occurred.");
         break;
     default:
        qDebug("An unknown QAbstractSocket::SocketError occurred.");
        break;
   }
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

bool QSslServer::openCertFile(QFile &certFile)
{
   if (!certFile.open(QIODevice::ReadOnly)) {
      qDebug("Could not open certificate file:");
      qDebug(certFile.fileName().toStdString().c_str());
      return false;
   }
   return true;
}
