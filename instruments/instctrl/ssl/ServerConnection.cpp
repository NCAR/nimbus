#include "ServerConnection.h"
#include <iostream>

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
ServerConnection::ServerConnection(SslSocket* sslSocket):
	_sslSocket(sslSocket)
{

	// react to changes in the state of the SslSocket
	connect(_sslSocket, SIGNAL(stateChanged(Ssl::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(Ssl::SslSocket::SocketState)));

	// Handle incoming messages from the SslSocket
	connect(_sslSocket, SIGNAL(readyRead()), this, SLOT(sslReadyRead()));

}

/////////////////////////////////////////////////////////////////////
ServerConnection::~ServerConnection() {
	if (_sslSocket) {
		_sslSocket->deleteLater();
	}
}

/////////////////////////////////////////////////////////////////////
void ServerConnection::socketStateChanged(Ssl::SslSocket::SocketState state) {

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

/////////////////////////////////////////////////////////////////////
void ServerConnection::sslReadyRead() {

	QByteArray data = _sslSocket->readAll();

	// Feed the characters to the protocol decoder.
	std::string s = QString(data).toStdString();
	std::vector<std::string> msgs = _protocolFromClient.incoming(s);

	for (int i = 0; i < msgs.size(); i++) {
		emit msgFromClient(msgs[i]);
	}
}

/////////////////////////////////////////////////////////////////////
bool ServerConnection::send(Protocols::Message& message) {

	qDebug() << __PRETTY_FUNCTION__;

	// Convert message to stringified JSON
	std::string m = message.toStdString();

	// Convert the json string to the protocol messages
	std::vector<std::string> msgs = _protocolToClient.outgoing(m);

	// Send the protocol messages to the server
	for (int i = 0; i < msgs.size(); i++ ) {
		std::string mm = msgs[i];
		int n = _sslSocket->write(mm.c_str());
		// Verify that the transmission succeeded
		if (n != mm.size()) {
			qDebug() << n << "bytes written from string of length" << mm.size();
			return false;
		}
	}

	// OK
	return true;
}
