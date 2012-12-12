#include "EmbeddedProxyServer.h"

/////////////////////////////////////////////////////////////////////
EmbeddedProxyServer::EmbeddedProxyServer(std::vector<std::string> proxyDefs):
_proxyDefs(proxyDefs)
{
	std::map<std::string, SslProxy::InstMsgInfo> messages;

	for (int i = 0; i < _proxyDefs.size(); i++) {
		QtConfig config(proxyDefs[i]);
		SslProxy::InstMsgInfo msg;
		msg._msgId                 = config.getString("InstMsgID",       "INST");
		msg._incomingPort          = config.getInt   ("InstIncomingPort",0);
		msg._destPort              = config.getInt   ("InstDestPort", 0);
		msg._destIP                = config.getString("InstHostName",    "127.0.0.1");
		msg._broadcast             = config.getBool  ("InstMsgBroadcast", true);
		msg._instName              = config.getString("InstName",    "INSTRUMENT");
		messages[msg._msgId] = msg;
	}

	_proxy = new EmbeddedProxy(messages);

	// Capture the proxy messages
	connect(_proxy, SIGNAL(msgFromProxy(std::string)), this, SLOT(msgFromProxySlot(std::string)));

}

/////////////////////////////////////////////////////////////////////
EmbeddedProxyServer::~EmbeddedProxyServer() {

}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxyServer::sendToProxy(Protocols::Message msg) {

}

/////////////////////////////////////////////////////////////////////
void EmbeddedProxyServer::msgFromProxySlot(std::string msg) {

	// Forward the proxy message
	emit msgFromProxy(msg);
	return;
}
