/*
 * ClientConnection.h
 *
 *  Created on: Nov 16, 2012
 *      Author: martinc
 */

#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

#include "SslSocket.h"
#include "Message.h"
#include "StreamMsgProtocol.h"

namespace Ssl {
	/// Manage a client connection to an SslServer using SslSocket. Capture the
	/// SslSocket::stateChanged() signal to react to the socket state.
	class ClientConnection: public QObject {
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
		ClientConnection(std::string keyFile,
				  std::string certFile,
				  std::string serverHost,
				  int port,
				  std::vector<std::string> caDatabase,
				  std::string clientID);

		/// Destructor
		virtual ~ClientConnection();
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
