#ifndef SSLCONNECTION_H_
#define SSLCONNECTION_H_

#include "SslSocket.h"
#include "Message.h"
#include "StreamMsgProtocol.h"

namespace Ssl {
	/// An SslConnection initiates an SSL connection to a server on behalf of a
	/// client or to establishes an SSL connection to a client on behalf of server
	/// when a client connection request is received.
	///
	/// The SslConnection creates an SslSocket and manages the protocols for
	/// communicating over the SSL link. The protocol is Protocols::StreamMsgProtocol,
	/// which serializes messages sent over a streaming (i.e. tcp) link.
	///
	/// When a complete message has been received from the SSL link, it is emitted
	/// as msgFromSslLink(Protocols::Message).  The send() function is called in order
	/// to send a message over the SSL link.
	///
	/// The SslConnection captures the SslSocket::socketStateChanged() signal, and
	/// simply re-emits it so that the object owner can take appropriate action.
	class SslConnection : public QObject {
		Q_OBJECT
	public:
		/// Initiate an SSL connection to a server
		/// @param keyFile Path to the file containing client private key.
		/// Specify a blank string if no key is provided.
		/// @param sslCert The certificate that matches client certificate.
		/// Specify a blank string if no certificate is provided.
		/// @param serverHost The server host name or IP address.
		/// @param port The server port number.
		/// @param extraCerts Extra certs that should be added to the CAdatabase
		/// @param clientID The client identifier
		SslConnection(std::string keyFile,
					  QSslCertificate sslCert,
					  std::string serverHost,
					  int port,
					  std::vector<QSslCertificate> extraCerts,
					  std::string clientID);
		/// Establish an SSL connection to a client
		/// @param sslSocket The encrypted SslSocket.
		SslConnection(Ssl::SslSocket* sslSocket);
		/// Destructor
		virtual ~SslConnection();

		/// Send a message over the SSL link
		/// @param message The message.
		/// @returns False if there was an error sending the message.
		bool send(Protocols::Message& message);
		/// @return The state of the connection.
		Ssl::SslSocket::SocketState connectionState();
		/// @return The peer certificate. The caller should verify that
		/// it is non-null and valid.
		QSslCertificate peerCertificate();
		/// @return The peer address of the remote ssl connection.
		QHostAddress peerAddress();
		/// Close the ssl connection
		void close();

	signals:
		/// Publish a change in the connection state. Currently this is
		/// simply the state of the underlying SslSocket.
		void connectionStateChanged(Ssl::SslConnection*, Ssl::SslSocket::SocketState);
		/// Emitted when a new message has been received over SSL link.
		/// @param msg The message.
		void msgFromSslLink(Protocols::Message msg);
		/// Emmitted when there has been an error in the connection.
		/// Right now just pass on the Qt socket error. Perhaps we will later change
		/// this to something specific to SslConnection.
		/// @param err The error code.
		/// @param s The associated error message.
		void connectionError(QAbstractSocket::SocketError err, std::string s);

	public slots:
		/// Capture a change in the SslSocket state.
		/// @param state The socket state
		void socketStateChanged(Ssl::SslSocket::SocketState);
		/// Called when an error is reported on the SslSocket.
		/// @param errorCode The error code.
		/// @param errMsg The error explanation.
		void sslSocketErrorSlot(QAbstractSocket::SocketError errorCode, std::string errMsg);
		/// New data are available on the SSL socket. Feed it to a protocol converter,
		/// and if complete, emit the message.
		void sslReadyRead();

	protected:
		/// The connected SSL socket
		Ssl::SslSocket* _sslSocket;
		/// True if this is a client initiated connection to a server
		bool _clientConnection;
		/// The protocol for messages sent over the SSL link
		Protocols::StreamMsgProtocol _protocolToSslLink;
		/// The protocol for messages received from the SSL link
		Protocols::StreamMsgProtocol _protocolFromSslLink;
		/// The peer certificate
		QSslCertificate _peerCertificate;
	};
};

#endif /* SSLCONNECTION_H_ */
