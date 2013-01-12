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
#include "InstConfig.h"

/// The server that manages connections with an SslProxy.
class SslProxyServer: public ProxyServer {
	Q_OBJECT;

	public:
		/// A helper class for defining a proxy.
		struct ProxyDef {
			QSslCertificate _sslCert;
			std::string _instConfigPath;
		};
		/// @param keyFile Path to the file containing the private key.
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// @param switchPort The server port number.
		/// @param proxies The allowable proxies.
	SslProxyServer(std::string sslKeyFile,
				QSslCertificate sslCertificate,
				int switchPort,
				std::vector<SslProxyServer::ProxyDef> proxies);
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
		/// Verify that a successfully encrypted connection is valid. If
		/// it isn't, close the connection, which will cause it to be removed
		/// from _connections. If it is valid, set up the book keeping so that
		/// the correct messaes are routed to it.
		/// @param connection The connection that will be validated.
		void validateConnection(Ssl::SslServerConnection* connection);
		/// The allowed proxies
		std::vector<SslProxyServer::ProxyDef> _proxies;
		/// The SSL server that receives SSL requests
		Ssl::SslServer* _sslServer;
		/// Keep track of active connections
		std::set<Ssl::SslServerConnection*> _connections;
};



#endif /* SWITCHSSLSERVER_H_ */
