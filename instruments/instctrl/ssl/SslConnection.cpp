/*
 * SslConnection.cpp
 */

#include "SslConnection.h"

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SslConnection::SslConnection(
		std::string keyFile,
		QSslCertificate sslCert,
		std::string serverHost,
		int port,
		std::vector<QSslCertificate> extraCerts,
		std::string clientID):
_sslSocket(0),
_clientConnection(true)
{
	_sslSocket = new SslSocket(keyFile, sslCert, serverHost, port, extraCerts);

	// react to changes in the state of the SslSocket
	connect(_sslSocket, SIGNAL(stateChanged(Ssl::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(Ssl::SslSocket::SocketState)));

	// capture error indications
	connect(_sslSocket, SIGNAL(sslSocketError(QAbstractSocket::SocketError, std::string)),
			this, SLOT(sslSocketErrorSlot(QAbstractSocket::SocketError, std::string)));

	// Handle incoming messages from the SslSocket
	connect(_sslSocket, SIGNAL(readyRead()), this, SLOT(sslReadyRead()));
}

/////////////////////////////////////////////////////////////////////
SslConnection::SslConnection(SslSocket* sslSocket) :
_sslSocket(sslSocket),
_clientConnection(false)
{
	// react to changes in the state of the SslSocket
	connect(_sslSocket, SIGNAL(stateChanged(Ssl::SslSocket::SocketState)),
			this, SLOT(socketStateChanged(Ssl::SslSocket::SocketState)));

	// Handle incoming messages from the SslSocket
	connect(_sslSocket, SIGNAL(readyRead()), this, SLOT(sslReadyRead()));
}

/////////////////////////////////////////////////////////////////////
SslConnection::~SslConnection()
{
	if (_sslSocket) {
		if (_clientConnection) {
			_sslSocket->close();
			delete _sslSocket;
		}
		else
			_sslSocket->deleteLater();
	}
}

/////////////////////////////////////////////////////////////////////
void SslConnection::socketStateChanged(Ssl::SslSocket::SocketState state)
{
	switch (state) {
		case SslSocket::SS_Unconnected:
		case SslSocket::SS_Connected:
		case SslSocket::SS_Disconnected:
			break;
		case SslSocket::SS_Encrypted:
			_peerCertificate = _sslSocket->peerCertificate();
			break;
		default:
			std::cout << "SslSocket changed to unknown state:" << state << std::endl;
			break;
	};

	// pass on the socket state change
	emit connectionStateChanged(this, state);
}

/////////////////////////////////////////////////////////////////////
void SslConnection::sslSocketErrorSlot(QAbstractSocket::SocketError errorCode, std::string errMsg)
{
	// Pass on the socket error
	emit connectionError(errorCode, errMsg);
}

/////////////////////////////////////////////////////////////////////
void SslConnection::sslReadyRead()
{
	QByteArray data = _sslSocket->readAll();

	// Feed the characters to the protocol decoder.
	std::string s = QString(data).toStdString();
	std::vector<std::string> msgs = _protocolFromSslLink.incoming(s);

	for (int i = 0; i < msgs.size(); i++) {
		emit msgFromSslLink(Protocols::Message(msgs[i]));
	}
}

/////////////////////////////////////////////////////////////////////
bool SslConnection::send(Protocols::Message& message)
{
	// Convert message to stringified JSON
	std::string m = message.toJsonStdString();

	// Convert the json string to the protocol messages
	std::vector<std::string> msgs = _protocolToSslLink.outgoing(m);

	// Send the protocol messages over the SSL link
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
Ssl::SslSocket::SocketState SslConnection::connectionState()
{
	if (!_sslSocket) {
		return (Ssl::SslSocket::SS_Unconnected);
	}
	return _sslSocket->state();
}

/////////////////////////////////////////////////////////////////////
QHostAddress SslConnection::peerAddress()
{
	QHostAddress addy;

	if (_sslSocket) {
		addy = _sslSocket->peerAddress();
	}

	return addy;
}

/////////////////////////////////////////////////////////////////////
QSslCertificate SslConnection::peerCertificate()
{
	return _peerCertificate;
}

/////////////////////////////////////////////////////////////////////
void SslConnection::close()
{
	if (_sslSocket) {
		_sslSocket->close();
	}
}
