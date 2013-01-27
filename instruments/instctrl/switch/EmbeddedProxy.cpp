#include "EmbeddedProxy.h"

/////////////////////////////////////////////////////////////////////
EmbeddedProxy::EmbeddedProxy(std::string Id,
		std::map<std::string, SslProxy::InstMsgInfo> messages) :
_Id(Id),
_messages(messages)
{
	initIncomingUDPsockets();

	initOutgoingUDPsocket();
}

/////////////////////////////////////////////////////////////////////
EmbeddedProxy::~EmbeddedProxy() {

}

/////////////////////////////////////////////////////////////////////
std::string EmbeddedProxy::Id() {
	return _Id;
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::initIncomingUDPsockets() {

	SslProxy::InstMsgInfo info = _messages.begin()->second;
	_incomingUdpPort = info._incomingPort;

	_incomingUdpSocket = new QUdpSocket(this);

	bool status = _incomingUdpSocket->bind(QHostAddress::LocalHost, _incomingUdpPort,
			QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	if (!status) {
		qDebug() << "Unable to bind to UDP port " << _incomingUdpPort;
		return;
	}

	connect(_incomingUdpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

	qDebug() << "Proxy" << _Id.c_str() << "will listen on port" << _incomingUdpPort;
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::initOutgoingUDPsocket() {

	_outgoingUdpSocket = new QUdpSocket(this);

	std::map<std::string, SslProxy::InstMsgInfo>::iterator it;
	for (it = _messages.begin(); it != _messages.end(); it++) {
		qDebug() << "Proxy" << _Id.c_str() << "will send" << it->second._msgId.c_str() <<
				"to" << it->second._destHost.c_str() << ":" << it->second._destPort
				<< (it->second._broadcast ?"BROADCAST" : "UNICAST");
	}
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::udpReadyRead() {

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
			SslProxy::InstMsgInfo info = _messages.begin()->second;
			Protocols::Message message(info._instName, id, text);

			qDebug() << message.toJsonStdString().c_str();

			// Tell the world that there is a new message!
			emit msgFromProxy(message);
		} else {
			/// @todo No identifier found in datagram; add error handling here
		}
	}
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::send(Protocols::Message msg) {

	// find this message in our message dictionary
	std::string msgId = msg.msgId();
	if (_messages.find(msgId) != _messages.end()) {
		sendMsg(_messages[msgId], msg);
	} else {
		/// @todo Handle appropriately; reporting/logging as needed.
		qDebug() << "Proxy" << _Id.c_str() << ": Unrecognized message"
				 << msgId.c_str() << "ignored by the proxy";
	}
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::sendMsg(SslProxy::InstMsgInfo& info, Protocols::Message& msg) {

	int sent;

	// Get the text of the message
	std::string text = msg.payload().text();

	qDebug() << text.c_str();

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
