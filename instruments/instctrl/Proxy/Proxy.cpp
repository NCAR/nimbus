#include "Proxy.h"
#include <iostream>

using namespace Ssl;
using namespace Protocols;

/////////////////////////////////////////////////////////////////////
Proxy::Proxy(int udpPort,
		std::string keyFile,
		std::string certFile,
		std::string serverHost,
		int switchPort,
		std::vector<std::string> caDatabase,
		std::string clientID):
_udpPort(udpPort),
_keyFile(keyFile),
_certFile(certFile),
_serverHost(serverHost),
_switchPort(switchPort),
_caDatabase(caDatabase),
_clientID(clientID),
_connection(0),
_udpSocket(0)
{

	initSwitchConnection();

	initUDPsocket();

}


/////////////////////////////////////////////////////////////////////
Proxy::~Proxy() {

	if (_udpSocket) {
		_udpSocket->deleteLater();
	}

	if (_connection) {
		delete _connection;
	}
}

/////////////////////////////////////////////////////////////////////
void Proxy::initSwitchConnection() {

	_connection = new ClientConnection(
			_keyFile,
			_certFile,
			_serverHost,
			_switchPort,
    		_caDatabase,
    		_clientID);

	connect(_connection, SIGNAL(msgFromServer(Protocols::Message)), this, SLOT(msgFromServerSlot(Protocols::Message)));

}

/////////////////////////////////////////////////////////////////////
void Proxy::initUDPsocket() {

	_udpSocket = new QUdpSocket(this);

	bool status = _udpSocket->bind(QHostAddress::LocalHost, _udpPort,
			QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	if (!status) {
		qDebug() << "unable to bind to UDP port " << _udpPort;
		return;
	}

	connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

}

/////////////////////////////////////////////////////////////////////
void Proxy::udpReadyRead() {
	while (_udpSocket->hasPendingDatagrams()) {

		// read the datagram
		int dataSize = _udpSocket->pendingDatagramSize();
		QByteArray data;
		data.resize(dataSize);
		_udpSocket->readDatagram(data.data(), dataSize);

		// create a Message
		Message msg(_clientID, _serverHost, QString(data).toStdString());

		// send the Message
		_connection->send(msg);
	}
}

/////////////////////////////////////////////////////////////////////
void Proxy::msgFromServerSlot(Protocols::Message msg) {
	qDebug() << msg.toStdString().c_str();
}
