#ifndef SERVER_H_
#define SERVER_H_

#include <QtCore>
#include <set>
#include "SslServer.h"
#include "ServerConnection.h"

class Server: public SSL::SslServer {
	Q_OBJECT;

	public:
	/// @param keyFile Path to the file containing the private key.
	/// @param certFile Path to the file containing the certificate that matched the private key.
	/// @param switchPort The server port number.
	/// @param caDatabase Paths to certs that should be added to the CAdatabase
	Server(std::string keyFile,
			std::string certFile,
			int switchPort,
			std::vector<std::string> caDatabase);
		/// Destructor.
		virtual ~Server();

	signals:

	protected slots:
		/// Called when a new SslSocket has been created.
		void createConnection(SSL::SslSocket* socket);
		/// Notify that the socket state has changed for a connection.
		void connectionStateChanged(SSL::ServerConnection*, SSL::SslSocket::SocketState);
		void messageFromClientSlot(std::string msg);

	protected:
		/// Keep track of active connections
		std::set<SSL::ServerConnection*> _connections;

};

#endif /* SERVER_H_ */
