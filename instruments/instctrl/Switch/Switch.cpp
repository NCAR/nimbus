#include "Switch.h"

/////////////////////////////////////////////////////////////////////
Switch::Switch(std::string keyFile,
		std::string certFile,
		int switchPort,
		std::vector<std::string> caDatabase,
		int localPort,
		std::string remoteIP,
		int remotePort,
		std::string switchCipherKey):
_server(keyFile, certFile, switchPort, caDatabase),
_switchConnection(localPort, remoteIP, remotePort, switchCipherKey)
{
	// The server emits all proxy messages. We will capture them.
	connect(&_server, SIGNAL(msgFromProxy(std::string)),
			this, SLOT(msgFromProxySlot(std::string)));

	// Capture the messages from the remote switch
	connect(&_switchConnection, SIGNAL(msgFromRemoteSwitch(Protocols::Message)),
			this, SLOT(msgFromRemoteSwitch(Protocols::Message)));


}

/////////////////////////////////////////////////////////////////////
Switch::~Switch() {

}

/////////////////////////////////////////////////////////////////////
void Switch::msgFromProxySlot(std::string msg) {

	// A message has been received from the proxy

	// send the proxy message to the remote switch
	_switchConnection.sendSwitchMessage(msg);

}

/////////////////////////////////////////////////////////////////////
void Switch::msgFromRemoteSwitch(Protocols::Message msg) {

	// A message has been received from the remote switch

	std::cout << __PRETTY_FUNCTION__ << std::endl;

	// Tell the server to forward the message to the proxy
	_server.sendToProxy(msg);

}
