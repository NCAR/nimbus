/*
 * Server.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: martinc
 */

#include "Server.h"
#include <assert.h>
#include <iostream>

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
	connect(connection, SIGNAL(connectionStateChanged(ServerConnection*, SSL::SslSocket::SocketState)),
			this, SLOT(connectionStateChanged(ServerConnection*, SSL::SslSocket::SocketState)));

	// add this connection to our list of connections.
	_connections.insert(connection);

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