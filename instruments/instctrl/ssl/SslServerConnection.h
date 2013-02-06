#ifndef SERVERCONNECTION_H_
#define SERVERCONNECTION_H_

#include "SslSocket.h"
#include "Message.h"
#include "StreamMsgProtocol.h"

namespace Ssl {
/// An SslServerConnection is created from a socket file descriptor,
/// when a client asks for an SSL connection.
/// An SslSocket is created, and manages the protocol for communicating with
/// the client. The protocol is Protocols::StreamMsgProtocol, which serializes
/// messages sent over a streaming (i.e. tcp) link.
///
/// The send() function is called in order to send a message to the client.
///
/// When a complete message has been received from the client, it is emitted
/// as msgFromClient(Protocols::Message).
///
/// SslServerConnection captures the SslSocket::socketStateChanged() signal,
/// and simply re-emits it so that the object owner can take appropriate action.
	class SslServerConnection: public QObject {
		Q_OBJECT
	public:
		/// @param sslSocket The encrypted SslSocket.
		SslServerConnection(Ssl::SslSocket* sslSocket);
		/// Destructor
		virtual ~SslServerConnection();
		/// Send a message to the client.
		/// @param message The message.
		/// @returns False if there was an error sending the message.
		bool send(Protocols::Message& message);
		/// @return The peer certificate. The caller should verify that
		/// it is non-null and valid.
		QSslCertificate peerCertificate();
		/// Close the ssl connection
		void close();
		/// @return The peer address of the remote ssl connection.
		QHostAddress peerAddress();

	signals:
		/// Publish a change in the connection state. Currently this is
		/// simply the state of the underlying SslSocket.
		void connectionStateChanged(Ssl::SslServerConnection*, Ssl::SslSocket::SocketState);
		/// Emitted when a new message has been received.
		/// @param msg The message.
		void msgFromClient(Protocols::Message);

	protected slots:
		/// Capture a change in the SslSocket state.
		void socketStateChanged(Ssl::SslSocket::SocketState);
		/// New data are available on the SSL socket. Feed it to a protocol converter,
		/// and if complete, emit the Protocol::Message.
		void sslReadyRead();

	protected:
		/// The connected SslSocket
		Ssl::SslSocket* _sslSocket;
		/// The protocol for communication to the server
		Protocols::StreamMsgProtocol _protocolToClient;
		/// The protocol for communication from the server
		Protocols::StreamMsgProtocol _protocolFromClient;
		/// The peer certificate
		QSslCertificate _peerCertificate;
	};
};

#endif /* SERVERCONNECTION_H_ */
