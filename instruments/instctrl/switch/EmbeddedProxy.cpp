#include "EmbeddedProxy.h"

/////////////////////////////////////////////////////////////////////
EmbeddedProxy::EmbeddedProxy(InstConfig& config, bool verbose) :
_verbose(verbose)
{
	_proxyId         = config.instrumentName();
	_incomingUdpPort = config.incomingPort();
	_destHost        = config.destHost();
	_destPort        = config.destPort();

	// Create a message entry for each message defined in the instrument configuration.
	std::vector<InstConfig::MessageInfo> instMessages = config.messages();
	std::vector<std::string> msgIds;
	for (int i = 0; i < instMessages.size(); i++) {
		std::cout << "Message id: " << instMessages[i].msgID << std::endl;
		std::string msgID = instMessages[i].msgID;
		_messages[msgID] = instMessages[i];
	}

	initIncomingUDPsockets();

	initOutgoingUDPsocket();
}

/////////////////////////////////////////////////////////////////////
EmbeddedProxy::~EmbeddedProxy()
{
}

/////////////////////////////////////////////////////////////////////
std::string EmbeddedProxy::Id()
{
	return _proxyId;
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::initIncomingUDPsockets()
{
	_incomingUdpSocket = new QUdpSocket(this);

	bool status = _incomingUdpSocket->bind(QHostAddress::Any, _incomingUdpPort,
			QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	if (!status) {
		qDebug() << "Unable to bind to UDP port " << _incomingUdpPort;
		return;
	}

	connect(_incomingUdpSocket, SIGNAL(readyRead()), this, SLOT(udpReadyRead()));

	qDebug() << "Proxy" << _proxyId.c_str() << "will listen on port" << _incomingUdpPort;
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::initOutgoingUDPsocket()
{
	_outgoingUdpSocket = new QUdpSocket(this);

	std::map<std::string, InstConfig::MessageInfo>::iterator it;
	for (it = _messages.begin(); it != _messages.end(); it++) {
		qDebug() << "Proxy" << _proxyId.c_str() << "will send" << it->second.msgID.c_str() <<
				"to" << _destHost.c_str() << ":" << _destPort
				<< (it->second.broadcast ?"BROADCAST" : "UNICAST");
	}
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::udpReadyRead()
{
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
		std::string msgId = Protocols::Message::extractId(text);
		if (msgId.size() > 0) {
			Protocols::Message message(_proxyId, msgId, text);

			if (_verbose)
				qDebug() << message.toJsonStdString().c_str();

			// Tell the world that there is a new message!
			emit msgFromProxy(message);
		} else {
			/// @todo No identifier found in datagram; add error handling here
		}
	}
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::send(Protocols::Message msg)
{
	// find this message in our message dictionary
	std::string msgId = msg.msgId();
	if (_messages.find(msgId) != _messages.end()) {
		sendMsg(_messages[msgId], msg);
	} else {
		/// @todo Handle appropriately; reporting/logging as needed.
		qDebug() << "Proxy" << _proxyId.c_str() << ": Unrecognized message"
				 << msgId.c_str() << "ignored by the proxy";
	}
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxy::sendMsg(InstConfig::MessageInfo& info, Protocols::Message& msg)
{
	int sent;

	// Get the text of the message
	std::string text = msg.payload().text();

	if (info.broadcast) {
		// message will be broadcast
		sent = _outgoingUdpSocket->writeDatagram(
				text.c_str(),
				text.size(),
				QHostAddress::Broadcast,
				_destPort);
	} else {
		// message will be unicast
		sent = _outgoingUdpSocket->writeDatagram(
				text.c_str(),
				text.size(),
				QtAddress::address(_destHost),
				_destPort);
	}
	if (sent != text.size()) {
		qDebug() << "Warning, only" << sent << "bytes out of" << text.size() <<
				"were sent to port" << _destPort << "for message ID " << info.msgID.c_str();
		if (sent == -1) {
			qDebug() << "The socket error is:" << _outgoingUdpSocket->error();
		}
	}
}
