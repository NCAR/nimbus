#ifndef SERVERCONNECTION_H_
#define SERVERCONNECTION_H_

#include "SslSocket.h"
#include "StreamMsgProtocol.h"

namespace Ssl {
/// Manage a connection to a client. The socket delivered to the constructor
/// must be in a connected state.
///
/// Incoming text on the socket are scanned for the JSON braces.
	class ServerConnection: public QObject {
		Q_OBJECT
	public:
		/// @param sslSocket The encrypted SslSocket.
		ServerConnection(Ssl::SslSocket* sslSocket);
		/// Destructor
		virtual ~ServerConnection();

	signals:
		/// Publish a change in the connection state. Currently this is
		/// simply the state of the underlying SslSocket.
		void connectionStateChanged(Ssl::ServerConnection*, Ssl::SslSocket::SocketState);
		/// Emitted when a new message has been received.
		/// @param msg The message.
		void messageFromClient(std::string msg);

	protected slots:
		/// Capture a change in the SslSocket state.
		void socketStateChanged(Ssl::SslSocket::SocketState);
		/// New data are available on the SSL socket. Append to _msgBuf, and scan
		/// for a complete JSON package. If complete, emit the JSON message.
		void sslReadyRead();

	protected:
		/// The connected SslSocket
		Ssl::SslSocket* _sslSocket;
		/// The protocol handler for proxy connections.
		Protocols::StreamMsgProtocol _protocol;
	};
};

#endif /* SERVERCONNECTION_H_ */
