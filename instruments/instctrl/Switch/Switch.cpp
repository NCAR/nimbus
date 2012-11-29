#include "Switch.h"

/////////////////////////////////////////////////////////////////////
Switch::Switch(std::string keyFile,
		std::string certFile,
		int switchPort,
		std::vector<std::string> caDatabase):
_server(keyFile, certFile, switchPort, caDatabase),
_p(EVPCipher::makeKey(16), false)
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

	// create an encrypted message
	std::vector<std::string> msgs = _p.outgoing(msg);

	// decode the encrypted message
	for (int i = 0; i < msgs.size(); i++) {
		std::vector<std::string> decrypted = _p.incoming(msgs[i]);
		for (int j = 0; j < decrypted.size(); j++) {
			qDebug() << "decrypted:" << decrypted[i].c_str();
		}
	}

}
