#include "SwitchSslServer.h"
#include <assert.h>
#include <iostream>

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SwitchSslServer::SwitchSslServer(std::string keyFile,
		std::string certFile,
		int port,
		std::vector<std::string> caDatabase):
		SslServer(keyFile, certFile, port, caDatabase)
{
	connect(this, SIGNAL(newConnection(Ssl::SslSocket*)),
			this, SLOT(createConnection(Ssl::SslSocket*)));

}

/////////////////////////////////////////////////////////////////////
SwitchSslServer::~SwitchSslServer() {

}

/////////////////////////////////////////////////////////////////////
void SwitchSslServer::createConnection(Ssl::SslSocket* sslSocket) {
	// make a new SwitchSslServer connection.
	SslServerConnection* connection = new SslServerConnection(sslSocket);

	// capture signals whrn the state of the connection changes.
	connect(connection, SIGNAL(connectionStateChanged(Ssl::SslSwitchSslServerConnection*, Ssl::SslSocket::SocketState)),
			this, SLOT(connectionStateChanged(Ssl::SslSwitchSslServerConnection*, Ssl::SslSocket::SocketState)));

	// add this connection to our list of connections.
	_connections.insert(connection);

	// capture new messages from this client
	connect(connection, SIGNAL(msgFromClient(std::string)), this, SLOT(msgFromProxySlot(std::string)));

	qDebug() << _connections.size() << " active SwitchSslServer connections";

}

/////////////////////////////////////////////////////////////////////
void SwitchSslServer::connectionStateChanged(SslServerConnection* connection, Ssl::SslSocket::SocketState state) {

	switch (state) {
	case SslSocket::SS_Unconnected:
	case SslSocket::SS_Connected:
	case SslSocket::SS_Encrypted:
	{
		// we don't care about these connections
		break;
	}
	case SslSocket::SS_Disconnected:
	case SslSocket::SS_SocketError: {

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
void SwitchSslServer::msgFromProxySlot(std::string msg) {
	emit msgFromProxy(msg);
	return;
}

/////////////////////////////////////////////////////////////////////
void SwitchSslServer::sendToProxy(Protocols::Message msg) {
	if (_connections.size() > 0) {
		qDebug() << __PRETTY_FUNCTION__;
		(*_connections.begin())->send(msg);
	}
}


