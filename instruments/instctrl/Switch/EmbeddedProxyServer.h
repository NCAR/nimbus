#ifndef SWITCHEMBEDDEDSERVER_H_
#define SWITCHEMBEDDEDSERVER_H_

#include <QtCore>
#include <string>
#include <vector>
#include "SwitchServer.h"
#include "EmbeddedProxy.h"

/// The SSL server that manages SSL connections with Proxy.
class EmbeddedProxyServer: public SwitchServer {
	Q_OBJECT;

	public:
		/// @param proxyDefs A list of proxies that we will create.
	EmbeddedProxyServer(std::vector<std::string> proxyDefs);
		/// Destructor.
		virtual ~EmbeddedProxyServer();
		/// Send a message to a client proxy
		virtual void sendToProxy(Protocols::Message msg);

	signals:
		void msgFromProxy(std::string msg);

	protected slots:
		/// A message has been received from the proxy
		void msgFromProxySlot(std::string msg);

	protected:
		std::vector<EmbeddedProxy*> _proxies;
};

#endif /* SWITCHEMBEDDEDSERVER_H_ */
