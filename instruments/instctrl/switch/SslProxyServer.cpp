#include "SslProxyServer.h"
#include <assert.h>
#include <iostream>

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SslProxyServer::SslProxyServer(std::string sslKeyFile,
		QSslCertificate sslCert,
		int port,
		std::vector<SslProxyServer::SslProxyDef> proxies):
_proxies(proxies)
{
	// A list of certificates that will be added to the CA database in order
	// to allow self-signed certificates to be accepted.
	std::vector<QSslCertificate> extraCerts;
	for (int i = 0; i < _proxies.size(); i++) {
		extraCerts.push_back(_proxies[i]._sslCert);
	}

	_sslServer = new SslServer(sslKeyFile, sslCert, port, extraCerts);

	// The newConnection is emitted by the SslServer when a new SSL connection
	// has been accepted from a proxy.
	_sslServer->connect(_sslServer, SIGNAL(newConnection(Ssl::SslSocket*)),
			this, SLOT(createConnection(Ssl::SslSocket*)));

	std::string msg("SslProxyServer started");
	_logger.log(msg);

}

/////////////////////////////////////////////////////////////////////
SslProxyServer::~SslProxyServer() {

}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::createConnection(Ssl::SslSocket* sslSocket) {

	// make a new SslServerConnection connection.
	SslServerConnection* connection = new SslServerConnection(sslSocket);

	// capture signals when the state of the connection changes.
	connect(connection, SIGNAL(connectionStateChanged(Ssl::SslServerConnection*, Ssl::SslSocket::SocketState)),
			this, SLOT(connectionStateChanged(Ssl::SslServerConnection*, Ssl::SslSocket::SocketState)));

	// add this connection to our list of connections.
	_connections.insert(connection);

	// capture new messages from this client
	connect(connection, SIGNAL(msgFromClient(Protocols::Message)), this, SLOT(msgFromProxySlot(Protocols::Message)));

}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::connectionStateChanged(SslServerConnection* connection,
		Ssl::SslSocket::SocketState state) {

	switch (state) {
	case SslSocket::SS_Unconnected:
	case SslSocket::SS_Connected:
	{
		// we don't care about these state changes
		break;
	}
	case SslSocket::SS_Encrypted:
	{
		// A successful SSL connection has been made. Make sure
		// that it is from a legitimate user.
		validateConnection(connection);
		break;
	}
	case SslSocket::SS_Disconnected:
	{
		/// Clean up the connection accounting.
		removeConnection(connection);
		break;
	}
	default: {
		// ignore unknown states
		break;
	}
	};
}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::validateConnection(Ssl::SslServerConnection* connection) {

	// Get the peer certificate
	QSslCertificate peerCert = connection->peerCertificate();

	std::vector<int> proxyIndices;
	if (!peerCert.isNull() && peerCert.isValid()) {
		// Look through the accepted certs and see if there is a match. Collect
		// the indices of all that match
		for (int i = 0; i < _proxies.size(); i++) {
			if (peerCert == _proxies[i]._sslCert) {
				proxyIndices.push_back(i);
			}
		}
	}

	if (proxyIndices.size() == 0) {
		// Either the certificate  is null, invalid, or not in our list.
		// Close the ssl connection, and it will trigger the SS_Disconnected
		// signal which will cause the connection to be removed.
		///@todo Add information about the rejected connection to the log message.
		_logger.log("Unrecognized or unavailable proxy certificate, connection closed");
		connection->close();
	} else {
		for (int i = 0; i < proxyIndices.size(); i++) {
			std::vector<InstConfig::MessageInfo> msgs = _proxies[i]._instConfig.messages();
			for (int j = 0; j < msgs.size(); j++) {
				std::string msgID = msgs[j].msgID;
				// Add this connection to the list for this message identifier.
				// If there is already a list for this message id, add the connection
				// to it. Otherwise,create a new list.
				if (_msgRouting.find(msgID) != _msgRouting.end()) {
					_msgRouting[msgID].insert(connection);
				} else {
					_msgRouting[msgID] = ConnectionList();
					_msgRouting[msgID].insert(connection);
				}
			}
		}

		// Send a message to the system log
		QString info = peerCert.subjectInfo(QSslCertificate::CommonName);
		QString IP = connection->peerAddress().toString();
		QString msg;
		msg = QString("SSL connection established for %1 at %2").arg(info).arg(IP);
		_logger.log(msg.toStdString());

		// And log all of the currently open connections.
		logOpenConnections();
	}
}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::removeConnection(Ssl::SslServerConnection* connection) {

	// get the peer certificate.
	QSslCertificate peerCert = connection->peerCertificate();

	// find it in our list of connections
	SslProxyServer::ConnectionList::iterator c = _connections.find(connection);

	// big problem if we have lost track of this connection
	if (c == _connections.end()) {
		std::string msg("Serious error, an unregistered connection has been disconnected.");
		std::cerr << msg << std::endl;
		_logger.log(msg);
	}

	// Get the IP
	QString IP = (*c)->peerAddress().toString();

	// kill the connection
	delete *c;

	// Remove all references to this connection in the _msgRouting list.
	std::map<std::string, ConnectionList >::iterator m;
	for (m = _msgRouting.begin(); m != _msgRouting.end(); m++) {
		m->second.erase(*c);
	}

	// If a message type has no more connections, get rid of it.
	for (m = _msgRouting.begin(); m != _msgRouting.end(); m++) {
		if (m->second.size() == 0) {
			_msgRouting.erase(m);
		}
	}

	// remove our record of the connection
	_connections.erase(c);

	// Notify the world that a disconnect has been received.
	QString info = peerCert.subjectInfo(QSslCertificate::CommonName);
	QString msg;
	msg = QString("SSL disconnect from %1 at %2").arg(info).arg(IP);
	_logger.log(msg.toStdString());

	// And log all of the currently open connections.
	logOpenConnections();
}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::logOpenConnections() {

	QString msg = QString("%1 active SSL connections ").arg(_connections.size());

	SslProxyServer::ConnectionList::iterator c;
	for (c = _connections.begin(); c != _connections.end(); c++) {
		QString peer = (*c)->peerAddress().toString();
		msg += " " + peer;
	}

	_logger.log(msg.toStdString());
}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::msgFromProxySlot(Protocols::Message message) {

	// Emit the Message
	emit msgFromProxy(message);

	return;
}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::sendToProxy(Protocols::Message msg) {
	// Get the message identifier
	std::string msgID = msg.msgId();

	// See if it is in our list of accepted messages
	if (_msgRouting.find(msgID) != _msgRouting.end()) {
		// It is a message we are interested in. Send it to the connections.
		for (ConnectionList::iterator i = _msgRouting[msgID].begin();
				i != _msgRouting[msgID].end(); i++) {
			(*i)->send(msg);
		}
	} else {
		// This is an unexpected message.
		/// @todo Maintain some book keeping and reporting on unexpected messages.
	}
}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::dumpCert( const QSslCertificate &cert )
{
    qDebug() << cert.toPem();

    qDebug() << "== Subject Info ==\b";
    qDebug() << "CommonName:\t\t" << cert.subjectInfo( QSslCertificate::CommonName );
    qDebug() << "Organization:\t\t" << cert.subjectInfo( QSslCertificate::Organization );
    qDebug() << "LocalityName:\t\t" << cert.subjectInfo( QSslCertificate::LocalityName );
    qDebug() << "OrganizationalUnitName:\t" << cert.subjectInfo( QSslCertificate::OrganizationalUnitName );
    qDebug() << "StateOrProvinceName:\t" << cert.subjectInfo( QSslCertificate::StateOrProvinceName );

    QMultiMap<QSsl::AlternateNameEntryType, QString> altNames = cert.alternateSubjectNames();
    if ( !altNames.isEmpty() ) {
        qDebug() << "Alternate Subject Names (DNS):";
        foreach (const QString &altName, altNames.values(QSsl::DnsEntry)) {
            qDebug() << altName;
        }

        qDebug() << "Alternate Subject Names (Email):";
        foreach (const QString &altName, altNames.values(QSsl::EmailEntry)) {
            qDebug() << altName;
        }
    }

    qDebug() << "== Issuer Info ==";
    qDebug() << "CommonName:\t\t" << cert.issuerInfo( QSslCertificate::CommonName );
    qDebug() << "Organization:\t\t" << cert.issuerInfo( QSslCertificate::Organization );
    qDebug() << "LocalityName:\t\t" << cert.issuerInfo( QSslCertificate::LocalityName );
    qDebug() << "OrganizationalUnitName:\t" << cert.issuerInfo( QSslCertificate::OrganizationalUnitName );
    qDebug() << "StateOrProvinceName:\t" << cert.issuerInfo( QSslCertificate::StateOrProvinceName );

    qDebug() << "== Certificate ==";
    //qDebug() << "Serial Number:\t\t" << cert.serialNumber(); // This seems buggy
    qDebug() << "Effective Date:\t\t" << cert.effectiveDate().toString();
    qDebug() << "Expiry Date:\t\t" << cert.expiryDate().toString();
    qDebug() << "Valid:\t\t\t" << (cert.isValid() ? "Yes" : "No");
}
