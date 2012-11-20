/*
 * ClientConnection.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: martinc
 */

#include "ClientConnection.h"

using namespace SSL;

/////////////////////////////////////////////////////////////////////
ClientConnection::ClientConnection(
		std::string keyFile,
		std::string certFile,
		std::string serverHost,
		int port,
		std::vector<std::string> caDatabase,
		std::string clientID) {

	_sslSocket = new SslSocket(keyFile, certFile, serverHost, port, caDatabase);

	connect(_sslSocket, SIGNAL(stateChanged(SSL::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(SSL::SslSocket::SocketState)));

}


/////////////////////////////////////////////////////////////////////
ClientConnection::~ClientConnection() {

}

/////////////////////////////////////////////////////////////////////
void ClientConnection::socketStateChanged(SSL::SslSocket::SocketState state) {

	switch (state) {
	case SslSocket::SS_Unconnected: {
		qDebug() << "socket is unconnected, what does this mean?";
		break;
	}
	case SslSocket::SS_Connected: {
		std::cout << "socket is connected" << std::endl;
		break;
	}
	case SslSocket::SS_Encrypted: {
		std::cout << "socket is encrypted" << std::endl;
		break;
	}
	case SslSocket::SS_Disconnected: {
		std::cout << "socket is disconnected, exiting" << std::endl;
		QCoreApplication::quit();
		break;
	}
	case SslSocket::SS_SocketError: {
		std::cout << "socket error, exiting" << std::endl;
		QCoreApplication::quit();
		break;
	}
	default: {
		std::cout << "socket changed to unknown state:" << state << std::endl;
		break;
	}
	};

}

/////////////////////////////////////////////////////////////////////
bool ClientConnection::send(Protocol::Message& message) {

	qDebug() << message.toStdString().c_str();

	// Convert message to stringified JSON
	std::string m = message.toStdString();

	// Send the JSON message to the server
	int n = _sslSocket->write(m.c_str());

	// Verify that the transmission succeeded
	if (n != m.size()) {
		qDebug() << n << "bytes written from string of length" << m.size();
		return false;
	}

	qDebug() << "wrote" << n << "bytes";
	// OK
	return true;
}
