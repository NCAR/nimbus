#ifndef SERVER_H_
#define SERVER_H_

#include <QtCore>
#include <set>
#include "SslServer.h"
#include "SslServerConnection.h"

/// The SSL server that manages SSL connections with Proxy.
class Server: public Ssl::SslServer {
	Q_OBJECT;

	public:
		/// @param keyFile Path to the file containing the private key.
		/// @param certFile Path to the file containing the certificate that matched the private key.
		/// @param switchPort The server port number.
		/// @param caDatabase Paths to certs that should be added to the CAdatabase
		Server(std::string keyFile,
				std::string certFile,
				int switchPort,
				std::vector<std::string> caDatabase);
		/// Destructor.
		virtual ~Server();
		/// Send a message to a client proxy
		virtual void sendToProxy(Protocols::Message msg) = 0;

	signals:
		void msgFromProxy(std::string msg);

	protected slots:
		/// A message has been received from the proxy
		void msgFromProxySlot(std::string msg) = 0;

	protected:

};

#endif /* SERVER_H_ */
