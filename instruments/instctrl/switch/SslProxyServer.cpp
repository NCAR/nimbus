#include "SslProxyServer.h"
#include <assert.h>
#include <iostream>

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SslProxyServer::SslProxyServer(std::string sslKeyFile,
		QSslCertificate sslCert,
		int port,
		std::vector<SslProxyServer::ProxyDef> proxies):
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

	qDebug() << _connections.size() << " active SwitchSslServer connections";

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

		// find it in our list of connections
		std::set<SslServerConnection*>::iterator p = _connections.find(connection);

		// big problem if we have lost track of this connection
		assert (p != _connections.end());

		// kill the connection
		delete connection;

		// remove our record of the connection
		_connections.erase(p);

		qDebug() << "connection is deleted, " << _connections.size() << " remaining connections";
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
		// signal which will cause the connection to be removed
		///@todo Error logging goes here
		std::cerr << "unrecognized or unavailable proxy certificate, connection closed" << std::endl;
		connection->close();
	} else {
		for (int i = 0; i < proxyIndices.size(); i++) {
			/// @todo Setup tables so that the correct messages get routed to this connection.
			std::cout << "Connection received for proxy " << i << std::endl;
		}
	}
}
/////////////////////////////////////////////////////////////////////
void SslProxyServer::msgFromProxySlot(Protocols::Message message) {

	// Emit the Message
	emit msgFromProxy(message);

	return;
}

/////////////////////////////////////////////////////////////////////
void SslProxyServer::sendToProxy(Protocols::Message msg) {
	if (_connections.size() > 0) {
		(*_connections.begin())->send(msg);
	}
}


