#include "EmbeddedProxyServer.h"

/////////////////////////////////////////////////////////////////////
EmbeddedProxyServer::EmbeddedProxyServer(std::vector<InstConfig> instConfigs,
										 bool verbose) throw(std::string)
{
	// Create one proxy for each instrument
	for (int i = 0; i < instConfigs.size(); i++) {
		InstConfig instConfig = instConfigs[i];
		std::cout << "Create proxy for " << instConfig.instrumentName() << std::endl;
		// Create a proxy for the instrument
		_proxy = new EmbeddedProxy(instConfig, verbose);

		// Build message routing table
		std::vector<InstConfig::MessageInfo> instMessages = instConfig.messages();
		for (int j = 0; j < instMessages.size(); j++) {
			_msgRouting[instMessages[j].msgID] = _proxy;
		}

		// Capture the proxy messages
		connect(_proxy, SIGNAL(msgFromProxy(Protocols::Message)), this, SLOT(msgFromProxySlot(Protocols::Message)));
	}
}

/////////////////////////////////////////////////////////////////////
EmbeddedProxyServer::~EmbeddedProxyServer()
{
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxyServer::sendToProxy(Protocols::Message message)
{
	// Get the message identifier
	std::string msgID = message.msgId();

	// See if it is in our list of accepted messages
	if (_msgRouting.find(msgID) != _msgRouting.end()) {
		// It is a message we are interested in. Send it via the proxy.
		_msgRouting[msgID]->send(message);
	} else {
		// This is an unexpected message.
		/// @todo Maintain some book keeping and reporting on unexpected messages.
	}
}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxyServer::msgFromProxySlot(Protocols::Message message)
{
	// Forward the proxy message
	emit msgFromProxy(message);
	return;
}
