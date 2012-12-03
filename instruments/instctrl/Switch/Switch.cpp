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
	connect(&_server, SIGNAL(msgFromProxy(std::string)),
			this, SLOT(msgFromProxySlot(std::string)));


}

/////////////////////////////////////////////////////////////////////
Switch::~Switch() {

}

/////////////////////////////////////////////////////////////////////
void Switch::msgFromProxySlot(std::string msg) {

	qDebug() << "Proxy msg:" << msg.c_str();

	// send the proxy message to the remote switch
	_switchConnection.sendSwitchMessage(msg);

}
