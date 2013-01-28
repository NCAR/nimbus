#include "SslProxy.h"
#include <iostream>

using namespace Ssl;
using namespace Protocols;

/////////////////////////////////////////////////////////////////////
SslProxy::SslProxy(
		std::string proxyID,
		std::string privateKeyFile,
		QSslCertificate sslCert,
		std::string serverHost,
		int switchPort,
		std::vector<QSslCertificate> extraCerts,
		std::map<std::string, InstMsgInfo> messages):
_proxyID(proxyID),
_keyFile(privateKeyFile),
_sslCert(sslCert),
_sslHost(serverHost),
_sslPort(switchPort),
_extraCerts(extraCerts),
_messages(messages),
_sslConnection(0),
_incomingUdpSocket(0),
_outgoingUdpSocket(0)
{
	// Add our certificate to the CAdatabase
	_extraCerts.push_back(sslCert);

	// Initialize the incoming UDP socket.
	initIncomingUDPsockets();

	// Initialize the outgoing UDP socket.
	initOutgoingUDPsocket();

}

/////////////////////////////////////////////////////////////////////
SslProxy::SslProxy(
		std::string proxyID,
		std::map<std::string, InstMsgInfo> messages):
_proxyID(proxyID),
_messages(messages),
_sslConnection(0),
_incomingUdpSocket(0),
_outgoingUdpSocket(0)
{

	// Initialize the incoming UDP socket.
	initIncomingUDPsockets();

	// Initialize the outgoing UDP socket.
	initOutgoingUDPsocket();

}

/////////////////////////////////////////////////////////////////////
SslProxy::~SslProxy() {

	// deleteLater() is used in cases where this delete may
	// be happening during a slot invocation, and there may be
	// pending signals for the object being deleted.

	if (_incomingUdpSocket) {
		_incomingUdpSocket->deleteLater();
	}

	if (_outgoingUdpSocket) {
		_outgoingUdpSocket->deleteLater();
	}

	if (_sslConnection) {
		delete _sslConnection;
	}
}

/////////////////////////////////////////////////////////////////////
void SslProxy::connectToServer() {

	openSslConnection();

}

/////////////////////////////////////////////////////////////////////
void SslProxy::disconnectFromServer() {

	closeSslConnection();

}

/////////////////////////////////////////////////////////////////////
void SslProxy::openSslConnection() {

	if (_sslConnection) {
		std::cerr << __PRETTY_FUNCTION__ << ": "
				<< "SSL connection requested while there is already an active SSL link" << std::endl;
		return;
	}

	// Initialize the connection to the switch. A SslClientConnection
	// will be created, and signals and slots will be connected.

	_sslConnection = new SslClientConnection(
			_keyFile,
			_sslCert,
			_sslHost,
			_sslPort,
    		_extraCerts,
    		_proxyID);

	connect(_sslConnection, SIGNAL(msgFromServer(Protocols::Message)), this, SLOT(msgFromServerSlot(Protocols::Message)));

}

/////////////////////////////////////////////////////////////////////
void SslProxy::closeSslConnection() {

	if (!_sslConnection) {
		return;
	}

	delete _sslConnection;
	_sslConnection = 0;

}

/////////////////////////////////////////////////////////////////////
void SslProxy::initIncomingUDPsockets() {

	_incomingUdpSocket = new QUdpSocket(this);

	/// @todo Actually, this will set up multiple sockets
	/// for all message types. Right, just use the port
	/// from the first message.
	int port = _messages.begin()->second._incomingPort;
	bool status = _incomingUdpSocket->bind(QHostAddress::LocalHost, port,
			QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	if (!status) {
		qDebug() << "unable to bind to UDP port " << port;
		return;
	}

	connect(_incomingUdpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

	qDebug() << "Proxy will listen on port" << port;

}

/////////////////////////////////////////////////////////////////////
void SslProxy::initOutgoingUDPsocket() {

	_outgoingUdpSocket = new QUdpSocket(this);

	std::map<std::string, InstMsgInfo>::iterator it;
	for (it = _messages.begin(); it != _messages.end(); it++) {
		qDebug() << "Proxy will send" << it->second._msgId.c_str() <<
				"to" << it->second._destHost.c_str() << ":" << it->second._destPort
				<< (it->second._broadcast ?"BROADCAST" : "UNICAST");
	}
}

/////////////////////////////////////////////////////////////////////
void SslProxy::udpReadyRead() {

	// A message has arrived from the instrument/controller
	while (_incomingUdpSocket->hasPendingDatagrams()) {

		// read the datagram
		int dataSize = _incomingUdpSocket->pendingDatagramSize();
		QByteArray data;
		data.resize(dataSize);
		_incomingUdpSocket->readDatagram(data.data(), dataSize);

		// create a Message
		std::string text = QString(data).toStdString();
		// Extract the message id from the datagram
		std::string id = Protocols::Message::extractId(text);
		if (id.size() > 0) {
			/// @todo Change the logic to search _messages to find the
			/// instrument name. Right now we will just use the first
			/// message that we have.
			Message msg(_proxyID, id, QString(data).toStdString());

			// send the message via the SSL connection.
			_sslConnection->send(msg);

			// Let others know about this message
			emit userMessage(text, true);
		} else {
			// Let others get this message
			emit userMessage(text, false);
		}
	}
}

/////////////////////////////////////////////////////////////////////
void SslProxy::msgFromServerSlot(Protocols::Message msg) {

	std::string msgId = msg.msgId();

	// find this message in our message dictionary
	if (_messages.find(msgId) != _messages.end()) {
		sendMsg(_messages[msgId], msg);
		emit switchMessage(msg.payload().text(), true);
	} else {
		emit switchMessage(msg.payload().text(), false);
	}


}

/////////////////////////////////////////////////////////////////////
void SslProxy::sendMsg(SslProxy::InstMsgInfo& info, Protocols::Message& msg) {

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
				info._destAddress,
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

