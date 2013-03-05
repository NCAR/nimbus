#ifndef EMBEDDEDPROXYSERVER_H_
#define EMBEDDEDPROXYSERVER_H_

#include <QtCore>
#include <string>
#include <vector>
#include "InstConfig.h"
#include "Message.h"
#include "ProxyServer.h"
#include "EmbeddedProxy.h"
#include "QtAddress.h"

/// The server that manages connections with an EmbeddedProxy.
class EmbeddedProxyServer: public ProxyServer {
	Q_OBJECT
public:
	/// @param instFiles A list of instrument configuration files.
	/// One proxy will be created for each instrument file.
	EmbeddedProxyServer(std::vector<InstConfig> instConfigs) throw(std::string);
	/// Destructor.
	virtual ~EmbeddedProxyServer();
	/// Send a message to a client proxy
	virtual void sendToProxy(Protocols::Message msg);

signals:
	void msgFromProxy(Protocols::Message message);
	void msgFromServer(Protocols::Message message);

protected slots:
	/// A message has been received from the proxy
	void msgFromProxySlot(Protocols::Message message);

protected:
	EmbeddedProxy* _proxy;
	/// Book keeping lists of message and the proxy that
	/// should be output. The key is the message ID
	std::map<std::string, EmbeddedProxy*> _msgRouting;

};

#endif /* SWITCHEMBEDDEDSERVER_H_ */
