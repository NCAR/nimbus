#ifndef EMBEDDEDPROXYSERVER_H_
#define EMBEDDEDPROXYSERVER_H_

#include <QtCore>
#include <string>
#include <vector>
#include "InstConfig.h"
#include "Message.h"
#include "SwitchServer.h"
#include "EmbeddedProxy.h"

/// The server that manages connections with an EmbeddedProxy.
class EmbeddedProxyServer: public SwitchServer {
	Q_OBJECT
	public:
		/// @param instFiles A list of instrument configuration files.
		/// One proxy will be created for each instrument file.
		EmbeddedProxyServer(std::vector<std::string> instFiles);
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
		std::vector<std::string> _instFiles;
};

#endif /* SWITCHEMBEDDEDSERVER_H_ */
