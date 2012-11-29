#ifndef SERVER_H_
#define SERVER_H_

#include <QtCore>
#include <set>
#include "SslServer.h"
#include "ServerConnection.h"

class Server: public Ssl::SslServer {
	Q_OBJECT;

	public:
		/// @param keyFile Path to the file containing the private key.
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// @param port The server listens on this port.
		/// @param caDatabase Paths to certs that should be added to the CAdatabase
		Server(std::string keyFile,
				std::string certFile,
				int port,
				std::vector<std::string> caDatabase);
		/// Destructor.
		virtual ~Server();

	protected slots:
		/// Called when a new SslSocket has been created.
		void createConnection(Ssl::SslSocket* socket);
		/// Notify that the socket state has changed for a connection.
		void connectionStateChanged(Ssl::ServerConnection*, Ssl::SslSocket::SocketState);

	protected:
		/// Keep track of active connections
		std::set<Ssl::ServerConnection*> _connections;

};

#endif /* SERVER_H_ */
