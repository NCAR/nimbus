/*
 * SslClientConnection.cpp
 *
 *  Created on: Nov 16, 2012
 *      Author: martinc
 */

#include "SslClientConnection.h"

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SslClientConnection::SslClientConnection(
		std::string keyFile,
		std::string certFile,
		std::string serverHost,
		int port,
		std::vector<std::string> caDatabase,
		std::string clientID) {

	_sslSocket = new SslSocket(keyFile, certFile, serverHost, port, caDatabase);

	// react to changes in the state of the SslSocket
	connect(_sslSocket, SIGNAL(stateChanged(Ssl::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(Ssl::SslSocket::SocketState)));

	// Handle incoming messages from the SslSocket
	connect(_sslSocket, SIGNAL(readyRead()), this, SLOT(sslReadyRead()));
}


/////////////////////////////////////////////////////////////////////
SslClientConnection::~SslClientConnection() {

}

/////////////////////////////////////////////////////////////////////
void SslClientConnection::socketStateChanged(Ssl::SslSocket::SocketState state) {

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
void SslClientConnection::sslReadyRead() {

	QByteArray data = _sslSocket->readAll();

	// Feed the characters to the protocol decoder.
	std::string s = QString(data).toStdString();
	std::vector<std::string> msgs = _protocolFromServer.incoming(s);

	for (int i = 0; i < msgs.size(); i++) {
		emit msgFromServer(Protocols::Message(msgs[i]));
	}
}

/////////////////////////////////////////////////////////////////////
bool SslClientConnection::send(Protocols::Message& message) {

	qDebug() << __PRETTY_FUNCTION__;

	// Convert message to stringified JSON
	std::string m = message.toJsonStdString();

	// Convert the json string to the protocol messages
	std::vector<std::string> msgs = _protocolToServer.outgoing(m);

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
