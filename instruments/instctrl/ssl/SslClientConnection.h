#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

#include "SslSocket.h"
#include "Message.h"
#include "StreamMsgProtocol.h"

namespace Ssl {
	/// An SslClientConnection initiates an SSL connection to a server.
	/// An SslSocket is created, and manages the protocol for communicating with
	/// the client. The protocol is Protocols::StreamMsgProtocol, which serializes
	/// messages sent over a streaming (i.e. tcp) link.
	///
	/// The send() function is called in order to send a message to the server.
	///
	/// When a complete message has been received from the server, it is emitted
	/// as msgFromServer(Protocols::Message).
	///
	/// SslClientConnection captures the SslSocket::socketStateChanged() signal,
	/// and simply re-emits it so that the object owner can take appropriate action.
	class SslClientConnection: public QObject {
		Q_OBJECT
	public:
		/// @param keyFile Path to the file containing the private key.
		/// Specify a blank string if no key is provided.
		/// @param sslCert The certificate that matches the private key.
		/// Specify a blank string if no certificate is provided.
		/// @param serverHost The server host name or IP address.
		/// @param port The server port number.
		/// @param extraCerts Extra certs that should be added to the CAdatabase
		/// @param clientID The client identifier
		SslClientConnection(std::string keyFile,
				  QSslCertificate sslCert,
				  std::string serverHost,
				  int port,
				  std::vector<QSslCertificate> extraCerts,
				  std::string clientID);

		/// Destructor
		virtual ~SslClientConnection();
		/// Send a message to the server.
		/// @param message The message.
		/// @returns False if there was an error sending the message.
		bool send(Protocols::Message& message);
		/// @return The state of the connection.
		Ssl::SslSocket::SocketState connectionState();

	signals:
		/// Publish a change in the connection state. Currently this is
		/// simply the state of the underlying SslSocket.
		void connectionStateChanged(Ssl::SslSocket::SocketState);
		/// Emitted when a new message has been received from the server.
		/// @param msg The message.
		void msgFromServer(Protocols::Message);
		/// Emmitted when there has been an error in the connection.
		/// Right now just pass on the Qt socket error. Perhaps we will later change
		/// this to something specific to SslClientConnection.
		void connectionError(QAbstractSocket::SocketError, std::string);

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
		/// The connected socket
		Ssl::SslSocket* _sslSocket;
		/// The protocol for communication to the server
		Protocols::StreamMsgProtocol _protocolToServer;
		/// The protocol for communication from the server
		Protocols::StreamMsgProtocol _protocolFromServer;
	};
};

#endif /* CLIENTCONNECTION_H_ */
