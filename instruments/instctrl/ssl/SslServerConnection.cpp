#include "SslServerConnection.h"
#include <iostream>

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SslServerConnection::SslServerConnection(SslSocket* sslSocket):
	_sslSocket(sslSocket)
{

	// react to changes in the state of the SslSocket
	connect(_sslSocket, SIGNAL(stateChanged(Ssl::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(Ssl::SslSocket::SocketState)));

	// Handle incoming messages from the SslSocket
	connect(_sslSocket, SIGNAL(readyRead()), this, SLOT(sslReadyRead()));

}

/////////////////////////////////////////////////////////////////////
SslServerConnection::~SslServerConnection() {
	if (_sslSocket) {
		_sslSocket->deleteLater();
	}
}

/////////////////////////////////////////////////////////////////////
void SslServerConnection::socketStateChanged(Ssl::SslSocket::SocketState state) {

	switch (state) {
	case SslSocket::SS_Unconnected: {
		break;
	}
	case SslSocket::SS_Connected: {
		break;
	}
	case SslSocket::SS_Encrypted: {
		_peerCertificate = _sslSocket->peerCertificate();
		break;
	}
	case SslSocket::SS_Disconnected: {
		break;
	}
	default: {
		std::cout << "SslSocket changed to unknown state:" << state << std::endl;
		break;
	}
	};

	// pass on the socket state change
	emit connectionStateChanged(this, state);
}

/////////////////////////////////////////////////////////////////////
void SslServerConnection::sslReadyRead() {

	QByteArray data = _sslSocket->readAll();

	// Feed the characters to the protocol decoder.
	std::string s = QString(data).toStdString();
	std::vector<std::string> msgs = _protocolFromClient.incoming(s);

	for (int i = 0; i < msgs.size(); i++) {
		Protocols::Message message(msgs[i]);
		emit msgFromClient(message);
	}
}

/////////////////////////////////////////////////////////////////////
bool SslServerConnection::send(Protocols::Message& message) {

	// Convert message to stringified JSON
	std::string m = message.toJsonStdString();

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

/////////////////////////////////////////////////////////////////////
QSslCertificate SslServerConnection::peerCertificate() {
	return _peerCertificate;
}
/////////////////////////////////////////////////////////////////////
void SslServerConnection::close() {
	if (_sslSocket) {
		_sslSocket->close();
	}
}
