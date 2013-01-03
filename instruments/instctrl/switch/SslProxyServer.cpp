#include "SslProxyServer.h"
#include <assert.h>
#include <iostream>

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SslProxyServer::SslProxyServer(std::string keyFile,
		std::string certFile,
		int port,
		std::vector<std::string> caDatabase)
//:SslServer(keyFile, certFile, port, caDatabase)
{
	_sslServer = new SslServer(keyFile, certFile, port, caDatabase);
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
void SslProxyServer::connectionStateChanged(SslServerConnection* connection, Ssl::SslSocket::SocketState state) {

	switch (state) {
	case SslSocket::SS_Unconnected:
	case SslSocket::SS_Connected:
	case SslSocket::SS_Encrypted:
	{
		// we don't care about these state changes
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


