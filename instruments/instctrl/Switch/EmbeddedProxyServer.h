#ifndef EMBEDDEDPROXYSERVER_H_
#define EMBEDDEDPROXYSERVER_H_

#include <QtCore>
#include <string>
#include <vector>
#include "QtConfig.h"
#include "Message.h"
#include "SwitchServer.h"
#include "EmbeddedProxy.h"

/// The server that manages connections with an EmbeddedProxy.
class EmbeddedProxyServer: public SwitchServer {
	Q_OBJECT
	public:
		/// @param proxyDefs A list of proxy configuration files.
	EmbeddedProxyServer(std::vector<std::string> proxyDefs);
		/// Destructor.
		virtual ~EmbeddedProxyServer();
		/// Send a message to a client proxy
		virtual void sendToProxy(Protocols::Message msg);

	signals:
		void msgFromProxy(std::string);

	protected slots:
		/// A message has been received from the proxy
		void msgFromProxySlot(std::string msg);

	protected:
		EmbeddedProxy* _proxy;
		std::vector<std::string> _proxyDefs;
};

#endif /* SWITCHEMBEDDEDSERVER_H_ */
