#ifndef SSLSOCKET_H_
#define SSLSOCKET_H_

#include <QtCore>
#include <QtNetwork>
#include <string>

/// SslSocket provides an SSL socket. It can be used either in server
/// or client mode.
/// In server mode, it is created from an already
/// open TCP connected socket, and will start the SSL handshake
/// using QSslSocket::startServerEncryption. Peer verification is
/// specified for server mode, so that the connected client must be
/// able to provide a valid certificate.
///
/// In client mode, SslSocket will initiate a socket connect to a host using
/// QSslSocket::connectToHostEncypted(). The client must have a valid
/// certificate that will be provided to the connecting server.
///
/// The caller supplies files containing the SSL private key and
/// certificate.
///
/// SslSocket will allow QSslError::SelfSignedCertificate errors,
/// but all other errors cause the connection to be disconnected.
class SslSocket : public QSslSocket
{
	Q_OBJECT;

public:
	/// Constructor for a server type socket. The descriptor will reference a connected
	/// socket which can be converted to a QSslSocket. startServerEncryption()
	/// will be issued.
	/// @param keyFile Path to the file containing the private key.
	/// Specify a blank string if no key is provided.
	/// @param certFile Path to the file containing the certificate that matched the private key.
	/// Specify a blank string if no certificate is provided.
	/// @param descriptor File descriptor for the connected socket.
	/// @param parent The Qt object parent.
	SslSocket(std::string keyFile, std::string certFile, int descriptor, QObject * parent = 0);
	/// Constructor for a client type socket. connectToHostEncrypted() will be issued.
	/// @param keyFile Path to the file containing the private key.
	/// Specify a blank string if no key is provided.
	/// @param certFile Path to the file containing the certificate that matched the private key.
	/// Specify a blank string if no certificate is provided.
	/// @param serverHost The server host name or IP address.
	/// @param port The server port number.
	/// @param parent The Qt object parent.
	SslSocket(std::string keyFile, std::string certFile, std::string serverHost, int port, QObject * parent = 0);
	/// Destructor
	virtual ~SslSocket();

protected slots:
	void connected();
	void disconnected();
	void encrypted();
	void sslErrors(QSslError& error);
	void sslErrors(const QList<QSslError>& errors);
	void modeChanged(QSslSocket::SslMode mode);

protected:
	/// Add the private key and certificate to the QsslSocket. Set up the signal/slot
	/// connections.
	void init();
	/// Path to the file containing the private key.
	std::string _keyFile;
	/// Path to the file containing the certificate.
	std::string _certFile;
	/// The connected socket file descriptor, for server applications.
	int _descriptor;
	/// The server port number, for client applications.
	int _port;
	/// The server host name or IP address, for client applications.
	std::string _serverHost;
};

#endif /* SSLSOCKET_H_ */
