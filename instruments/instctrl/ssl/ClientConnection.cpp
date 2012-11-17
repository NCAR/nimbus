/*
 * ClientConnection.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: martinc
 */

#include "ClientConnection.h"
#include <iostream>

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
		std::cout << "socket is unconnected, what does this mean?" << std::endl;
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
