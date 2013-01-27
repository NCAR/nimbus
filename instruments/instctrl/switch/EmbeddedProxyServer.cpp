#include "EmbeddedProxyServer.h"

/////////////////////////////////////////////////////////////////////
EmbeddedProxyServer::EmbeddedProxyServer(std::vector<InstConfig> instConfigs) throw(std::string):
_instConfigs(instConfigs)
{
	for (int i = 0; i < _instConfigs.size(); i++) {
		InstConfig instConfig = _instConfigs[i];
		std::map<std::string, SslProxy::InstMsgInfo> messages;

		// Create a message entry for each message defined in the instrument configuration.
		std::vector<InstConfig::MessageInfo> instMessages = instConfig.messages();
		std::vector<std::string> msgIds;
		for (int i = 0; i < instMessages.size(); i++) {
			SslProxy::InstMsgInfo msg;
			msg._instName              = instConfig.instrumentName();
			msg._incomingPort          = instConfig.incomingPort();
			msg._destHost              = instConfig.destHost();
			msg._destPort              = instConfig.destPort();
			msg._msgId                 = instMessages[i].msgID;
			msg._broadcast             = instMessages[i].broadcast;
			msg._destAddress           = QtAddress::address(instConfig.destHost());

			std::cout << "Message id: " << msg._msgId << std::endl;
			messages[msg._msgId] = msg;
			msgIds.push_back(instMessages[i].msgID);
		}

		_proxy = new EmbeddedProxy(instConfig.instrumentName(), messages);

		// Capture the proxy messages
		connect(_proxy, SIGNAL(msgFromProxy(Protocols::Message)), this, SLOT(msgFromProxySlot(Protocols::Message)));

		// Build message routing table
		for (int j = 0; j < msgIds.size(); j++)
			_msgRouting[msgIds[j]] = _proxy;
	}
}

/////////////////////////////////////////////////////////////////////
EmbeddedProxyServer::~EmbeddedProxyServer() {
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxyServer::sendToProxy(Protocols::Message message) {
	// Get the message identifier
	std::string msgID = message.msgId();

	// See if it is in our list of accepted messages
	if (_msgRouting.find(msgID) != _msgRouting.end()) {
		//std::cout << "Routing " << msgID << " via proxy " << _msgRouting[msgID]->Id() << std::endl;
		// It is a message we are interested in. Send it via the proxy.
		_msgRouting[msgID]->send(message);
	} else {
		// This is an unexpected message.
		/// @todo Log the unexpected message.
		std::cout << "There is no proxy registered for " << msgID << std::endl;
	}
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxyServer::msgFromProxySlot(Protocols::Message message) {
	// Forward the proxy message
	emit msgFromProxy(message);
	return;
}
