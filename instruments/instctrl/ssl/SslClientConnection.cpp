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
		QSslCertificate sslCert,
		std::string serverHost,
		int port,
		std::vector<QSslCertificate> extraCerts,
		std::string clientID):
_sslSocket(0)
{

	_sslSocket = new SslSocket(keyFile, sslCert, serverHost, port, extraCerts);

	// react to changes in the state of the SslSocket
	connect(_sslSocket, SIGNAL(stateChanged(Ssl::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(Ssl::SslSocket::SocketState)));

	// capture error indications
	connect(_sslSocket,
			SIGNAL(sslSocketError(QAbstractSocket::SocketError, std::string)),
			this,
			SLOT(sslSocketErrorSlot(QAbstractSocket::SocketError, std::string)));

	// Handle incoming messages from the SslSocket
	connect(_sslSocket, SIGNAL(readyRead()), this, SLOT(sslReadyRead()));
}


/////////////////////////////////////////////////////////////////////
SslClientConnection::~SslClientConnection() {

	if (_sslSocket) {
		_sslSocket->close();
		delete _sslSocket;
	}

}

/////////////////////////////////////////////////////////////////////
void SslClientConnection::socketStateChanged(Ssl::SslSocket::SocketState state) {

	// just pass on the socket state change
	emit connectionStateChanged(state);
}

/////////////////////////////////////////////////////////////////////
void SslClientConnection::sslSocketErrorSlot(QAbstractSocket::SocketError errorCode,
		std::string errMsg) {

	// Pass on the socket error
	emit connectionError(errorCode, errMsg);

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

/////////////////////////////////////////////////////////////////////
Ssl::SslSocket::SocketState SslClientConnection::connectionState() {
	if (!_sslSocket) {
		return (Ssl::SslSocket::SS_Unconnected);
	}
	return _sslSocket->state();
}

