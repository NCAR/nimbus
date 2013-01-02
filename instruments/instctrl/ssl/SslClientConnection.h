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
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// Specify a blank string if no certificate is provided.
		/// @param serverHost The server host name or IP address.
		/// @param port The server port number.
		/// @param caDatabase Paths to certs that should be added to the CAdatabase
		/// @param clientID The client identifier
		SslClientConnection(std::string keyFile,
				  std::string certFile,
				  std::string serverHost,
				  int port,
				  std::vector<std::string> caDatabase,
				  std::string clientID);

		/// Destructor
		virtual ~SslClientConnection();
		/// Send a message to the server.
		/// @param message The message.
		/// @returns False if there was an error sending the message.
		bool send(Protocols::Message& message);

	signals:
		/// Emitted when a new message has been received from the server.
		/// @param msg The message.
		void msgFromServer(Protocols::Message);

	public slots:
		/// Capture a change in the SslSocket state.
		void socketStateChanged(Ssl::SslSocket::SocketState);
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
