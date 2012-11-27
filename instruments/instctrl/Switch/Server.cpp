#include "Server.h"
#include <assert.h>
#include <iostream>
#include "SymCipherProtocol.h"

using namespace SSL;

/////////////////////////////////////////////////////////////////////
Server::Server(std::string keyFile,
		std::string certFile,
		int port,
		std::vector<std::string> caDatabase):
		SslServer(keyFile, certFile, port, caDatabase)
{
	connect(this, SIGNAL(newConnection(SSL::SslSocket*)),
			this, SLOT(createConnection(SSL::SslSocket*)));

}

/////////////////////////////////////////////////////////////////////
Server::~Server() {

}

/////////////////////////////////////////////////////////////////////
void Server::createConnection(SSL::SslSocket* sslSocket) {
	// make a new server connection.
	ServerConnection* connection = new ServerConnection(sslSocket);

	// capture signals whrn the state of the connection changes.
	connect(connection, SIGNAL(connectionStateChanged(SSL::ServerConnection*, SSL::SslSocket::SocketState)),
			this, SLOT(connectionStateChanged(SSL::ServerConnection*, SSL::SslSocket::SocketState)));

	// add this connection to our list of connections.
	_connections.insert(connection);

	// capture new messages from this client
	connect(connection, SIGNAL(messageFromClient(std::string)), this, SLOT(messageFromClientSlot(std::string)));

	qDebug() << _connections.size() << " active server connections";
}

/////////////////////////////////////////////////////////////////////
void Server::connectionStateChanged(ServerConnection* connection, SSL::SslSocket::SocketState state) {

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
		std::set<ServerConnection*>::iterator p = _connections.find(connection);

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
void Server::messageFromClientSlot(std::string msg) {
	qDebug() << "Server msg:" << msg.c_str();
	Protocols::SymCipherProtocol p;
	std::vector<std::string> msgs = p.outgoing(msg);
	for (int i = 0; i < msgs.size(); i++) {
		qDebug() << "encrypted:" << msgs[i].c_str();
	}
}


