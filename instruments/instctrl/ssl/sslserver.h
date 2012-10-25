#ifndef SSLSERVER_H_
#define SSLSERVER_H_

#include <QtCore>
#include <QtNetwork>
#include "sslsocket.h"

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
	/// @param parent The Qt object parent.
	SslServer(std::string keyFile, std::string certFile, int port, QObject * parent = 0);
	/// Destructor.
	virtual ~SslServer();

protected slots:

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
};

#endif /* SSLSERVER_H_ */
