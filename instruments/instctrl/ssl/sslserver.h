#ifndef SSLSERVER_H_
#define SSLSERVER_H_

#include <QtCore>
#include <QtNetwork>
#include "sslsocket.h"

/// SslServer creates a server which listens for connections on a specific
/// port. When a connection is made, the socket is converted to a
/// SslSocket, which then carries out the SSL handshake.
///
/// The caller supplies files containing the server private key and
/// certificate.
class SslServer : public QTcpServer
{
	Q_OBJECT;

public:
	SslServer(std::string keyFile, std::string certFile, int port, QObject * parent = 0);
	virtual ~SslServer();

protected slots:

protected:
	void incomingConnection(int socketDescriptor);
	std::string _keyFile;
	std::string _certFile;
	int _port;

};

#endif /* SSLSERVER_H_ */
