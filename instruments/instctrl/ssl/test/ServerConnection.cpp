#include "ServerConnection.h"
#include <iostream>

using namespace SSL;

/////////////////////////////////////////////////////////////////////
ServerConnection::ServerConnection(SslSocket* sslSocket):
	_sslSocket(sslSocket)
{

	connect(_sslSocket, SIGNAL(stateChanged(SSL::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(SSL::SslSocket::SocketState)));

}

/////////////////////////////////////////////////////////////////////
ServerConnection::~ServerConnection() {

}

/////////////////////////////////////////////////////////////////////
void ServerConnection::socketStateChanged(SSL::SslSocket::SocketState state) {

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
		std::cout << "socket is disconnected" << std::endl;
		break;
	}
	case SslSocket::SS_SocketError: {
		std::cout << "socket error" << std::endl;
		break;
	}
	default: {
		std::cout << "socket changed to unknown state:" << state << std::endl;
		break;
	}
	};

	// pass on the socket state change
	emit connectionStateChanged(this, state);
}
