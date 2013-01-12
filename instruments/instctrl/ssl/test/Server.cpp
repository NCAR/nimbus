/*
 * Server.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: martinc
 */

#include "Server.h"
#include <assert.h>
#include <iostream>

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
Server::Server(std::string keyFile,
		QSslCertificate sslCert,
		int port,
		std::vector<QSslCertificate> extraCerts):
		SslServer(keyFile, sslCert, port, extraCerts)
{
	connect(this, SIGNAL(newConnection(Ssl::SslSocket*)),
			this, SLOT(createConnection(Ssl::SslSocket*)));
}

/////////////////////////////////////////////////////////////////////
Server::~Server() {

}

/////////////////////////////////////////////////////////////////////
void Server::createConnection(Ssl::SslSocket* sslSocket) {
	// make a new server connection.
	SslServerConnection* connection = new SslServerConnection(sslSocket);

	// capture signals whrn the state of the connection changes.
	connect(connection, SIGNAL(connectionStateChanged(Ssl::SslServerConnection*, Ssl::SslSocket::SocketState)),
			this, SLOT(connectionStateChanged(Ssl::SslServerConnection*, Ssl::SslSocket::SocketState)));

	// add this connection to our list of connections.
	_connections.insert(connection);

	qDebug() << _connections.size() << " active server connections";
}

/////////////////////////////////////////////////////////////////////
void Server::connectionStateChanged(SslServerConnection* connection, Ssl::SslSocket::SocketState state) {

	switch (state) {
	case SslSocket::SS_Unconnected:
	case SslSocket::SS_Connected:
	case SslSocket::SS_Encrypted:
	{
		// we don't care about these connections
		break;
	}
	case SslSocket::SS_Disconnected:{

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
