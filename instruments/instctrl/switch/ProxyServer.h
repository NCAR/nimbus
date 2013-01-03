#ifndef SWITCHSERVER_H_
#define SWITCHSERVER_H_

#include <QtCore>
#include <string>
#include "Message.h"

/// An abstract class that manages proxy instances and communications with the
/// proxies. In some derived classes, such as SslProxyServer, it creates proxies
/// on demand. In other derived uses, such as EmbeddedProxyServer, it will create
/// the specified proxies at startup.
class ProxyServer: public QObject {
	Q_OBJECT

	public:
		/// Constructor.
		ProxyServer();
		/// Destructor.
		virtual  ~ProxyServer();
		/// Send a message to a proxy.
		virtual void sendToProxy(Protocols::Message message) = 0;
		/// A message has been received from a proxy.
		virtual void msgFromProxySlot(Protocols::Message message) = 0;
};

#endif /* SWITCHSERVER_H_ */
