#ifndef SWITCHSERVER_H_
#define SWITCHSERVER_H_

#include <QtCore>
#include <string>
#include "Message.h"

/// The switch server that manages SSL connections with Proxy.
class SwitchServer: public QObject {
	Q_OBJECT

	public:
	SwitchServer();
	virtual  ~SwitchServer();
	/// Send a message to a client proxy
	virtual void sendToProxy(Protocols::Message message) = 0;

	//	protected slots:
	/// A message has been received from the proxy
	virtual void msgFromProxySlot(Protocols::Message message) = 0;

};

#endif /* SWITCHSERVER_H_ */
