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
	/// using QSslSocket::startServerEncryption(). Peer verification is
	/// specified for server mode, so that the connected client must be
	/// able to provide a valid certificate.
	///
	/// In client mode, SslSocket will initiate a socket connect to a host using
	/// QSslSocket::connectToHostEncypted(). The client must have a valid
	/// certificate that will be transmitted to the connecting server.
	///
	/// The caller supplies files containing the SSL private key and
	/// certificate.
	///
	/// Self-signed certificates will generate SSL errors, unless the incoming
	/// certificates are in the Certificate Authority database. The caDatabase
    /// parameter to the constructor allows expected certificates to be added
    /// for this purpose.
	class SslSocket : public QSslSocket
	{
		Q_OBJECT;
	public:
		/// The state of SslSocket.
		enum SocketState {
			SS_Unconnected = 0,
			SS_Connected,
			SS_Encrypted,
			SS_Disconnected
		};

		/// Constructor for a server type socket. The descriptor will reference a connected
		/// socket which can be converted to a QSslSocket. startServerEncryption() will be issued.
		/// @param keyFile Path to the file containing the private key.
		/// Specify a blank string if no key is provided.
		/// @param sslCert The certificate that matches the private key.
		/// Specify a blank string if no certificate is provided.
		/// @param descriptor File descriptor for the connected socket.
		/// @param extraCerts Extra certs that should be added to the CAdatabase
		/// @param parent The Qt object parent.
		SslSocket(std::string keyFile,
				  QSslCertificate sslCert,
				  int descriptor,
				  std::vector<QSslCertificate> extraCerts,
				  QObject * parent = 0);
		/// Constructor for a client type socket. connectToHostEncrypted() will be issued.
		/// @param keyFile Path to the file containing the private key.
		/// Specify a blank string if no key is provided.
		/// @param sslCert The certificate that matches the private key.
		/// Specify a blank string if no certificate is provided.
		/// @param serverHost The server host name or IP address.
		/// @param port The server port number.
		/// @param extraCerts Extra certs that should be added to the CAdatabase
		/// @param parent The Qt object parent.
		SslSocket(std::string keyFile,
				  QSslCertificate sslCert,
				  std::string serverHost,
				  int port,
				  std::vector<QSslCertificate> extraCerts,
				  QObject * parent = 0);
		/// Destructor
		virtual ~SslSocket();
		/// @returns The state of the socket.
		SocketState state();

	signals:
		/// Emitted when socket state changes
		void stateChanged(Ssl::SslSocket::SocketState);
		/// Emitted when a socket error is reported:
		void sslSocketError(QAbstractSocket::SocketError errorCode, std::string errMsg);

	protected slots:
		/// Called when the socket is connected. Emit stateChanged(SS_Connected);
		void connected();
		/// Called when the socket is disconnected. Emit stateChanged(SS_Disconnected).
		void disconnected();
		/// Called when there is an error on the socket. Emit stateChanged(SS_SocketError).
		void socketError(QAbstractSocket::SocketError error);
		/// Called when the socket correctly enters the encrypted state. Emit stateChanged(SS_Encrypted).
		void encrypted();
		/// Called when SSL errors are encountered. At present does nothing.
		/// Since the errors are not handled here, if they are critical, the socket
		/// will be automatically disconnected during the handshake sequence.
		void sslErrors(const QList<QSslError>& errors);
		/// Called when the socket mode changes. Does nothing.
		void modeChanged(QSslSocket::SslMode mode);

	protected:
		/// Add the private key and certificate to the QsslSocket. Set up the signal/slot connections.
		void init();
		/// Set the CA database from the list of certificates in _extraCerts.
		void setCAdatabase();
		/// Connect the signals.
		void connectSignals();
		/// Dump the certificate database
		void dumpCA();

		/// Path to the file containing the private key.
		std::string _keyFile;
		/// The SSL certificate.
		QSslCertificate _sslCert;
		/// The connected socket file descriptor, for server applications.
		int _descriptor;
		/// The server port number, for client applications.
		int _port;
		/// The server host name or IP address, for client applications.
		std::string _serverHost;
		/// Current socket state
		SocketState _state;
		/// Certificates that will be added to the CAdatabase
		std::vector<QSslCertificate> _extraCerts;
	};
};

#endif /* SSLSOCKET_H_ */
