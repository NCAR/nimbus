#ifndef SSLSERVER_H_
#define SSLSERVER_H_

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <map>
#include "SslSocket.h"
#include "RicLogger.h"

namespace Ssl {
	/// SslServer creates a server which listens for connections on a specific
	/// port. When a connection is made, the socket is converted to a
	/// SslSocket, which then carries out the SSL handshake. A newConnection(Ssl::SslSocket* sslSocket)
	/// signal is emitted when the new connection is established. All of the
	/// QTcpServer base class signals and member functions are available for
    /// implementing a complete server.
	///
	/// The caller supplies files containing the server private key and
	/// certificate.
	class SslServer : public QTcpServer
	{
		Q_OBJECT;

	public:
		/// @param keyFile Path to the file containing the private key.
		/// @param sslCert The certificate that matches the private key.
		/// @param port The server listens on this port.
		/// @param extraCerts Extra certs that should be added to the CAdatabase
		/// @param parent The Qt object parent.
		SslServer(std::string keyFile,
				QSslCertificate sslCert,
				int port,
				std::vector<QSslCertificate> extraCerts,
				QObject * parent = 0);
		/// Destructor.
		virtual ~SslServer();

	signals:
		/// Emitted when a new connection is made
		/// @param sslSocket Pointer to the new socket
		void newConnection(Ssl::SslSocket* sslSocket);

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
		QSslCertificate _sslCert;
		/// The server listens on this port.
		int _port;
		/// Certs that will be added to the CAdatabase
		std::vector<QSslCertificate> _extraCerts;
		/// The list of created server sockets. Use the address of the socket
		/// to reference it.
		std::map<SslSocket*, SslSocket*> _sslSockets;
		/// The system message logger
		RicLogger _logger;
	};
};

#endif /* SSLSERVER_H_ */
