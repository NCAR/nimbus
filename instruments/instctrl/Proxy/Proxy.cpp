#include "Proxy.h"
#include <iostream>

using namespace Ssl;
using namespace Protocols;

/////////////////////////////////////////////////////////////////////
Proxy::Proxy(
		int udpPort,
		std::string keyFile,
		std::string certFile,
		std::string serverHost,
		int switchPort,
		std::vector<std::string> caDatabase,
		std::string instName,
		std::map<std::string, InstMsgInfo> messages):
_udpPort(udpPort),
_keyFile(keyFile),
_certFile(certFile),
_serverHost(serverHost),
_switchPort(switchPort),
_caDatabase(caDatabase),
_instName(instName),
_messages(messages),
_connection(0),
_incomingUdpSocket(0),
_outgoingUdpSocket(0)
{

	// Initialize the connection to the switch. A ClientConnection
	// will be created, and signals and slots will be connected.
	initSwitchConnection();

	// Initialize the incoming UDP socket.
	initIncomingUDPsockets();

	// Initialize the outgoing UDP socket.
	initOutgoingUDPsocket();

}


/////////////////////////////////////////////////////////////////////
Proxy::~Proxy() {

	if (_incomingUdpSocket) {
		_incomingUdpSocket->deleteLater();
	}

	if (_outgoingUdpSocket) {
		_outgoingUdpSocket->deleteLater();
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
    		_instName);

	connect(_connection, SIGNAL(msgFromServer(Protocols::Message)), this, SLOT(msgFromServerSlot(Protocols::Message)));

}

/////////////////////////////////////////////////////////////////////
void Proxy::initIncomingUDPsockets() {

	_incomingUdpSocket = new QUdpSocket(this);

	bool status = _incomingUdpSocket->bind(QHostAddress::LocalHost, _udpPort,
			QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	if (!status) {
		qDebug() << "unable to bind to UDP port " << _udpPort;
		return;
	}

	connect(_incomingUdpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

}

/////////////////////////////////////////////////////////////////////
void Proxy::initOutgoingUDPsocket() {

	_outgoingUdpSocket = new QUdpSocket(this);

}

/////////////////////////////////////////////////////////////////////
void Proxy::udpReadyRead() {

	// A message has arrived from the instrument/controller
	while (_incomingUdpSocket->hasPendingDatagrams()) {

		// read the datagram
		int dataSize = _incomingUdpSocket->pendingDatagramSize();
		QByteArray data;
		data.resize(dataSize);
		_incomingUdpSocket->readDatagram(data.data(), dataSize);

		// create a Message
		/// @todo Actually, this should extract the message ID from the
		/// incoming string, and verify that it exists in
		/// _messages. Right now we will just use the first
		/// message that we have.
		std::string msgId = _messages.begin()->second._msgId;
		Message msg(_instName, msgId, QString(data).toStdString());

		// send the Message
		_connection->send(msg);
	}
}

/////////////////////////////////////////////////////////////////////
void Proxy::msgFromServerSlot(Protocols::Message msg) {
	qDebug() << msg.payload().text().c_str();

	std::string msgId = msg.msgId();

	// find this message in our message dictionary
	if (_messages.find(msgId) != _messages.end()) {
		broadcastMsg(_messages[msgId], msg);
	} else {
		/// @todo Handle appropriately; reporting/logging as needed.
		qDebug() << "Proxy: Unrecognized message" << msgId.c_str() << "ignored by the proxy";
	}

}

/////////////////////////////////////////////////////////////////////
void Proxy::broadcastMsg(Proxy::InstMsgInfo& info, Protocols::Message& msg) {

	int sent;

	// Get the text of the message
	std::string text = msg.payload().text();

	if (info._broadcast) {
		// message will be broadcast
		sent = _outgoingUdpSocket->writeDatagram(
				text.c_str(),
				text.size(),
				QHostAddress::Broadcast,
				info._destPort);
	} else {
		// message will be unicast
		sent = _outgoingUdpSocket->writeDatagram(
				text.c_str(),
				text.size(),
				QHostAddress(info._destIP.c_str()),
				info._destPort);
	}
	if (sent != text.size()) {
		qDebug() << "Warning, only" << sent << "bytes out of" << text.size() <<
				"were sent to port" << info._destPort << "for message ID " << info._msgId.c_str();
		if (sent == -1) {
			qDebug() << "The socket error is:" << _outgoingUdpSocket->error();
		}
	}
}

