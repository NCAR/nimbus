#ifndef SWITCH_H_H
#define SWITCH_H_H

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>

#include "Server.h"
#include "SymCipherProtocol.h"

/// The application which manages communications between one or more
/// instances of Proxy and another Switch. The mutiple Proxy clients
/// may be aggregated within a Switch, or may be remote. When Proxy is
/// remote, it communicates via an SslSocket. When it is aggregated
/// withing Switch, there is a direct connection to Proxy.
class Switch: public QObject {
	Q_OBJECT
public:
	/// @param keyFile Path to the file containing the private key.
	/// @param certFile Path to the file containing the certificate that matched the private key.
	/// @param switchPort The server port number.
	/// @param caDatabase Paths to certs that should be added to the CAdatabase
	Switch(std::string keyFile,
			std::string certFile,
			int switchPort,
			std::vector<std::string> caDatabase);

	virtual ~Switch();
	
protected slots:
	void msgFromProxySlot(std::string msg);

protected:
	/// The server which creates new ServerConnections.
	Server _server;
	Protocols::SymCipherProtocol _p;


};

#endif

