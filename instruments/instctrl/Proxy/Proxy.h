#ifndef PROXY_H_H
#define PROXY_H_H

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>

#include "Message.h"
#include "ClientConnection.h"

/// Proxy ties together an SSL connection to a server and a local UDP socket.
class Proxy: public QObject {
	Q_OBJECT
public:
	/// @param udpPort The incoming udp port
	/// @param keyFile Path to the file containing the private key.
	/// Specify a blank string if no key is provided.
	/// @param certFile Path to the file containing the certificate that matched the private key.
	/// Specify a blank string if no certificate is provided.
	/// @param serverHost The server host name or IP address.
	/// @param switchPort The server port number.
	/// @param caDatabase Paths to certs that should be added to the CAdatabase
	/// @param clientID The client identifier
	Proxy(int udpPort,
			std::string keyFile,
			std::string certFile,
			std::string serverHost,
			int switchPort,
			std::vector<std::string> caDatabase,
			std::string clientID);

	virtual ~Proxy();
	
protected slots:
	void udpReadyRead();
	void msgFromServerSlot(Protocols::Message);

protected:
	/// Initialize the connection to the switch
	void initSwitchConnection();
	/// initialize the incoming UDP socket
	void initUDPsocket();

	/// Port number for incoming datagrams
	int _udpPort;
	/// Path to the private key.
	std::string _keyFile;
	/// Path to the client certificate.
	std::string _certFile;
	/// The name or IP of the switch.
	std::string _serverHost;
	/// The switch port number.
	int _switchPort;
	/// Paths to extra certificates to be added to the database
	std::vector<std::string> _caDatabase;
	/// The client identifier
	std::string _clientID;
	/// The connection to the switch
	Ssl::ClientConnection* _connection;
	/// The datagram socket
	QUdpSocket* _udpSocket;

};

#endif
