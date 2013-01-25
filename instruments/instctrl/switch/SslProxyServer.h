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
		/// A helper class for defining an SSL proxy.
		struct SslProxyDef {
			QSslCertificate _sslCert;
			InstConfig _instConfig;
		};
		/// @param keyFile Path to the file containing the private key.
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// @param switchPort The server port number.
		/// @param proxies The allowable proxies.
		SslProxyServer(std::string sslKeyFile,
				QSslCertificate sslCertificate,
				int switchPort,
				std::vector<SslProxyServer::SslProxyDef> proxies);
		/// Destructor.
		virtual ~SslProxyServer();
		/// Send a message to a client proxy. The _msgRouting list
		/// is searched for the message id. If found, the message is
		/// sent to all of the SslServerConnections associated with
		/// the message id.
		virtual void sendToProxy(Protocols::Message msg);

	signals:
		void msgFromProxy(Protocols::Message msg);

	protected slots:
		/// Called when a new SslSocket has been created.
		void createConnection(Ssl::SslSocket* socket);
		/// Notify that the socket state has changed for a connection.
		void connectionStateChanged(Ssl::SslServerConnection*, Ssl::SslSocket::SocketState);
		/// Receive a proxy message. Emit it as: msgFromProxy(Protocols::Message)
		/// @param msg The message
		void msgFromProxySlot(Protocols::Message message);

	protected:
		/// A list of connections.
		typedef std::multiset<Ssl::SslServerConnection*> ConnectionList;
		/// Verify that a successfully encrypted connection is valid. If
		/// it isn't, close the connection, which will cause it to be removed
		/// from _connections. If it is valid, set up the book keeping so that
		/// the correct messages will be routed to it.
		/// @param connection The connection that will be validated.
		void validateConnection(Ssl::SslServerConnection* connection);
		/// The allowed proxies.
		std::vector<SslProxyServer::SslProxyDef> _proxies;
		/// The SSL server that receives SSL requests
		Ssl::SslServer* _sslServer;
		/// Keep track of active connections
		ConnectionList _connections;
		/// Book keeping which lists all connections which will output a
		/// a designated message. The key is the message id. The ConnectionList
		/// contains pointers to all SslServerConnections that these messages
		/// should be sent to.
		std::map<std::string, ConnectionList > _msgRouting;
};



#endif /* SWITCHSSLSERVER_H_ */
