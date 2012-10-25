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
	SslSocket(std::string keyFile, std::string certFile, int descriptor, QObject * parent = 0);
	/// Constructor for a client type socket. connectToHostEncrypted() will be issued.
	SslSocket(std::string keyFile, std::string certFile, std::string serverHost, int port, QObject * parent = 0);
	/// Destructor
	virtual ~SslSocket();

protected slots:
	void connected();
	void disconnected();
	void encrypted();
	void sslErrors(const QList<QSslError>& errors);
protected:
	void init();
	std::string _keyFile;
	std::string _certFile;
	int _descriptor;
	int _port;
	std::string _serverHost;
};

#endif /* SSLSOCKET_H_ */
