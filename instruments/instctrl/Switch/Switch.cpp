#include "Switch.h"

/////////////////////////////////////////////////////////////////////
Switch::Switch(std::string keyFile,
		std::string certFile,
		int switchPort,
		std::vector<std::string> caDatabase):
_server(keyFile, certFile, switchPort, caDatabase),
_p(true)
{
	connect(&_server, SIGNAL(messageFromClient(std::string)),
			this, SLOT(messageFromClientSlot(std::string)));
}

/////////////////////////////////////////////////////////////////////
Switch::~Switch() {

}

/////////////////////////////////////////////////////////////////////
void Switch::messageFromClientSlot(std::string msg) {
	qDebug() << "Switch msg:" << msg.c_str();
	std::vector<std::string> msgs = _p.outgoing(msg);
	for (int i = 0; i < msgs.size(); i++) {
		qDebug() << "encrypted:" << msgs[i].c_str();
		std::vector<std::string> decrypted = _p.incoming(msgs[i]);
		for (int j = 0; j < decrypted.size(); j++) {
			qDebug() << "decrypted:" << decrypted[i].c_str();
		}
	}

}
