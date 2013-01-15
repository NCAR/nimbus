#include "EmbeddedProxyServer.h"

/////////////////////////////////////////////////////////////////////
EmbeddedProxyServer::EmbeddedProxyServer(std::vector<InstConfig> instConfigs):
_instConfigs(instConfigs)
{
	std::map<std::string, SslProxy::InstMsgInfo> messages;

	for (int i = 0; i < _instConfigs.size(); i++) {
		InstConfig instConfig = _instConfigs[i];

		// Create a message entry for each message defined in the instrument configuration.
		std::vector<InstConfig::MessageInfo> instMessages = instConfig.messages();
		for (int i = 0; i < instMessages.size(); i++) {
			SslProxy::InstMsgInfo msg;
			msg._instName              = instConfig.instrumentName();
			msg._incomingPort          = instConfig.incomingPort();
			msg._destIP                = instConfig.destIP();
			msg._destPort              = instConfig.destPort();
			msg._msgId                 = instMessages[i].msgID;
			msg._broadcast             = instMessages[i].broadcast;
			std::cout << "message id:" << msg._msgId << std::endl;
			messages[msg._msgId] = msg;
		}
	}

	_proxy = new EmbeddedProxy(messages);

	// Capture the proxy messages
	connect(_proxy, SIGNAL(msgFromProxy(Protocols::Message)), this, SLOT(msgFromProxySlot(Protocols::Message)));

}

/////////////////////////////////////////////////////////////////////
EmbeddedProxyServer::~EmbeddedProxyServer() {

}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxyServer::sendToProxy(Protocols::Message message) {
	_proxy->send(message);
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxyServer::msgFromProxySlot(Protocols::Message message) {

	// Forward the proxy message
	emit msgFromProxy(message);
	return;
}
