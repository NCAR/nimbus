#ifndef SSLSERVER_H_
#define SSLSERVER_H_

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <map>
#include "SslSocket.h"

namespace SSL {
	/// SslServer creates a server which listens for connections on a specific
	/// port. When a connection is made, the socket is converted to a
	/// SslSocket, which then carries out the SSL handshake.
	///
	/// The caller supplies files containing the server private key and
	/// certificate.
	class SslServer : public QTcpServer
	{
		Q_OBJECT;

	public:
		/// @param keyFile Path to the file containing the private key.
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// @param port The server listens on this port.
		/// @param caDatabase Paths to certs that should be added to the CAdatabase
		/// @param parent The Qt object parent.
		SslServer(std::string keyFile,
				std::string certFile,
				int port,
				std::vector<std::string> caDatabase,
				QObject * parent = 0);
		/// Destructor.
		virtual ~SslServer();

	signals:
		/// Emitted when a new connection is made
		/// @param descriptor Pointer to the new socket
		void newConnection(SSL::SslSocket* sslSocket);

	protected slots:
		/// List the created server sockets
		void showServerSockets();

	protected:
		/// Called when the server has made a socket connection to an
		/// incoming request.
		/// @param descriptor File descriptor for the connected socket.
		void incomingConnection(int descriptor);

		/// Path to the file containing the private key.
		std::string _keyFile;
		/// Path to the file containing the certificate that matched the private key.
		std::string _certFile;
		/// The server listens on this port.
		int _port;
		/// Paths to certs that will be added to the CAdatabase
		std::vector<std::string> _caDatabase;
		/// The list of created server sockets. Use the address of the socket
		/// to reference it.
		std::map<SslSocket*, SslSocket*> _sslSockets;
	};
};

#endif /* SSLSERVER_H_ */
