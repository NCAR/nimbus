#ifndef SSLSOCKET_H_
#define SSLSOCKET_H_

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>
namespace Ssl {

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
		/// The state of SslSocket
		enum SocketState {
			SS_Unconnected = 0,
			SS_Connected,
			SS_Encrypted,
			SS_Disconnected,
			SS_SocketError
		};

		/// Constructor for a server type socket. The descriptor will reference a connected
		/// socket which can be converted to a QSslSocket. startServerEncryption() will be issued.
		/// @param keyFile Path to the file containing the private key.
		/// Specify a blank string if no key is provided.
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// Specify a blank string if no certificate is provided.
		/// @param descriptor File descriptor for the connected socket.
		/// @param caDatabase Paths to certs that should be added to the CAdatabase
		/// @param parent The Qt object parent.
		SslSocket(std::string keyFile,
				  std::string certFile,
				  int descriptor,
				  std::vector<std::string> caDatabase,
				  QObject * parent = 0);
		/// Constructor for a client type socket. connectToHostEncrypted() will be issued.
		/// @param keyFile Path to the file containing the private key.
		/// Specify a blank string if no key is provided.
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// Specify a blank string if no certificate is provided.
		/// @param serverHost The server host name or IP address.
		/// @param port The server port number.
		/// @param caDatabase Paths to certs that should be added to the CAdatabase
		/// @param parent The Qt object parent.
		SslSocket(std::string keyFile,
				  std::string certFile,
				  std::string serverHost,
				  int port,
				  std::vector<std::string> caDatabase,
				  QObject * parent = 0);
		/// Destructor
		virtual ~SslSocket();
		SocketState state();
		std::string socketID();

	signals:
		/// Emitted when socket state changes
		void stateChanged(Ssl::SslSocket::SocketState);

	protected slots:
		void connected();
		void disconnected();
		void socketError(QAbstractSocket::SocketError error);
		void encrypted();
		void sslErrors(const QList<QSslError>& errors);
		void modeChanged(QSslSocket::SslMode mode);

	protected:
		/// Add the private key and certificate to the QsslSocket. Set up the signal/slot connections.
		void init();
		/// set the CA database from the list of certificates in _caDatabase.
		void setCAdatabase();
		/// Connect the signals.
		void connectSignals();
		/// Dump the certificate database
		void dumpCA();

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
		/// Current socket state
		SocketState _state;
		/// Paths to certs that will be added to the CAdatabase
		std::vector<std::string> _caDatabase;
		/// A text description of the socket
		std::string _socketID;
	};
};

#endif /* SSLSOCKET_H_ */
