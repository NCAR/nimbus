/*
 * SslProxyServer.h
 *
 *  Created on: Dec 10, 2012
 *      Author: martinc
 */

#ifndef SWITCHSSLSERVER_H_
#define SWITCHSSLSERVER_H_


#include <QtCore>
#include <set>
#include "ProxyServer.h"
#include "SslServer.h"
#include "SslServerConnection.h"

/// The server that manages connections with an SslProxy.
class SslProxyServer: public ProxyServer {
	Q_OBJECT;

	public:
		/// @param keyFile Path to the file containing the private key.
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// @param switchPort The server port number.
		/// @param caDatabase Paths to certs that should be added to the CAdatabase
	SslProxyServer(std::string keyFile,
				std::string certFile,
				int switchPort,
				std::vector<std::string> caDatabase);
		/// Destructor.
		virtual ~SslProxyServer();
		/// Send a message to a client proxy
		virtual void sendToProxy(Protocols::Message msg);

	signals:
		void msgFromProxy(Protocols::Message msg);

	protected slots:
		/// Called when a new SslSocket has been created.
		void createConnection(Ssl::SslSocket* socket);
		/// Notify that the socket state has changed for a connection.
		void connectionStateChanged(Ssl::SslServerConnection*, Ssl::SslSocket::SocketState);
		/// Receive a proxy message. Emit it as
		/// msgFromProxy(Protocols::Message msg)
		/// @param msg The message
		void msgFromProxySlot(Protocols::Message message);

	protected:
		Ssl::SslServer* _sslServer;
		/// Keep track of active connections
		std::set<Ssl::SslServerConnection*> _connections;
};



#endif /* SWITCHSSLSERVER_H_ */