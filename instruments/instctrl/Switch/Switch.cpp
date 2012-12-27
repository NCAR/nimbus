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
_switchConnection(localPort, remoteIP, remotePort, switchCipherKey),
_server(0)
{

	// Create an SSL proxy server
	_server = new SslProxyServer(keyFile, certFile, switchPort, caDatabase);

	// Initialize
	init();
}

/////////////////////////////////////////////////////////////////////
Switch::Switch(std::vector<std::string> proxySpecs,
		int localPort,
		std::string remoteIP,
		int remotePort,
		std::string switchCipherKey):
_switchConnection(localPort, remoteIP, remotePort, switchCipherKey),
_server(0)
{
	// Create an embedded proxy server
	_server = new EmbeddedProxyServer(proxySpecs);

	// Initialize
	init();
}

/////////////////////////////////////////////////////////////////////
Switch::~Switch() {
	delete _server;
}

/////////////////////////////////////////////////////////////////////
void Switch::init() {

	// The server emits all proxy messages. We will capture them.
	connect(_server, SIGNAL(msgFromProxy(Protocols::Message)),
			this, SLOT(msgFromProxySlot(Protocols::Message)));

	// Capture the messages from the remote switch
	connect(&_switchConnection, SIGNAL(msgFromRemoteSwitch(Protocols::Message)),
			this, SLOT(msgFromRemoteSwitch(Protocols::Message)));

}
/////////////////////////////////////////////////////////////////////
void Switch::msgFromProxySlot(Protocols::Message message) {

	std::cout << message.toJsonStdString() << std::endl;

	// A message has been received from the proxy
	std::string msg = message.toJsonStdString();

	// send the proxy message to the remote switch
	_switchConnection.sendSwitchMessage(msg);

}

/////////////////////////////////////////////////////////////////////
void Switch::msgFromRemoteSwitch(Protocols::Message message) {

	// A message has been received from the remote switch

	std::cout << message.toJsonStdString() << std::endl;

	// Tell the server to forward the message to the proxy
	_server->sendToProxy(message);

}
