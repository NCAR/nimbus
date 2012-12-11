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
#include "SwitchServer.h"
#include "SslServer.h"
#include "SslServerConnection.h"

/// The server that manages connections with an SslProxy.
class SslProxyServer: public SwitchServer {
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
		void msgFromProxy(std::string msg);

	protected slots:
		/// Called when a new SslSocket has been created.
		void createConnection(Ssl::SslSocket* socket);
		/// Notify that the socket state has changed for a connection.
		void connectionStateChanged(Ssl::SslServerConnection*, Ssl::SslSocket::SocketState);
		/// A message has been received from the proxy
		void msgFromProxySlot(std::string msg);

	protected:
		Ssl::SslServer* _sslServer;
		/// Keep track of active connections
		std::set<Ssl::SslServerConnection*> _connections;
};



#endif /* SWITCHSSLSERVER_H_ */
