#ifndef SERVERCONNECTION_H_
#define SERVERCONNECTION_H_

#include "SslSocket.h"

/// Manage a connection to a client. The socket delivered to the constructor
/// must be in a connected state.
class ServerConnection: public QObject {
	Q_OBJECT
public:
	/// @param sslSocket The encrypted SslSocket.
	ServerConnection(SSL::SslSocket* sslSocket);

	virtual ~ServerConnection();

signals:
    /// Publish a change in the connection state. Currently this is
    /// simply the state of the underlying SslSocket.
	void connectionStateChanged(ServerConnection*, SSL::SslSocket::SocketState);

protected slots:
    /// Capture a change in the SslSocket state.
	void socketStateChanged(SSL::SslSocket::SocketState);

protected:
	/// The connected SslSocket
	SSL::SslSocket* _sslSocket;
};

#endif /* SERVERCONNECTION_H_ */
