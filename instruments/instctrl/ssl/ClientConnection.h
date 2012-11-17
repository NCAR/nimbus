/*
 * ClientConnection.h
 *
 *  Created on: Nov 16, 2012
 *      Author: martinc
 */

#ifndef CLIENTCONNECTION_H_
#define CLIENTCONNECTION_H_

#include "SslSocket.h"

namespace SSL {
	/// Create a client connection to an SslServer using SslSocket. Capture the
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

		virtual ~ClientConnection();

	public slots:
		void socketStateChanged(SSL::SslSocket::SocketState);

	protected:
		/// The connected socket
		SSL::SslSocket* _sslSocket;

	};
};

#endif /* CLIENTCONNECTION_H_ */
