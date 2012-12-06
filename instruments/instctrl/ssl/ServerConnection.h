#ifndef SERVERCONNECTION_H_
#define SERVERCONNECTION_H_

#include "SslSocket.h"
#include "Message.h"
#include "StreamMsgProtocol.h"

namespace Ssl {
/// Manage a server connection to a client using SslSocket. Capture the
/// SslSocket::stateChanged() signal to react to the socket state. The socket
/// delivered to the constructor must be in a connected state.
	class ServerConnection: public QObject {
		Q_OBJECT
	public:
		/// @param sslSocket The encrypted SslSocket.
		ServerConnection(Ssl::SslSocket* sslSocket);
		/// Destructor
		virtual ~ServerConnection();
		/// Send a message to the client.
		/// @param message The message.
		/// @returns False if there was an error sending the message.
		bool send(Protocols::Message& message);

	signals:
		/// Publish a change in the connection state. Currently this is
		/// simply the state of the underlying SslSocket.
		void connectionStateChanged(Ssl::ServerConnection*, Ssl::SslSocket::SocketState);
		/// Emitted when a new message has been received.
		/// @param msg The message.
		void msgFromClient(std::string msg);

	protected slots:
		/// Capture a change in the SslSocket state.
		void socketStateChanged(Ssl::SslSocket::SocketState);
		/// New data are available on the SSL socket. Feed it to a protocol converter,
		/// and if complete, emit the message.
		void sslReadyRead();

	protected:
		/// The connected SslSocket
		Ssl::SslSocket* _sslSocket;
		/// The protocol for communication to the server
		Protocols::StreamMsgProtocol _protocolToClient;
		/// The protocol for communication from the server
		Protocols::StreamMsgProtocol _protocolFromClient;
	};
};

#endif /* SERVERCONNECTION_H_ */
