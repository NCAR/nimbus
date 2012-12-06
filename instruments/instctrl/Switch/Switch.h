#ifndef SWITCH_H_H
#define SWITCH_H_H

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>

#include "Server.h"
#include "SwitchConnection.h"
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
	/// @param localPort The port that we listen for incoming messages on.
	/// @param remoteIP The remote IP that we send messages to
	/// @param remotePort The port that we send messages to
	/// @param switchCipherKey The file containing the key for symmetric cipher encryption over SwitchConnection.

	Switch(std::string keyFile,
			std::string certFile,
			int switchPort,
			std::vector<std::string> caDatabase,
			int _localPort,
			std::string _remoteIP,
			int _remotePort,
			std::string switchCipherKey);

	virtual ~Switch();
	
protected slots:
	/// Handle a new message from the proxy
	/// @param The incoming message from the proxy.
	void msgFromProxySlot(std::string msg);
	/// Handle a new message from the remote switch
	/// @param The incoming message from the remote switch.
	void msgFromRemoteSwitch(Protocols::Message msg);

protected:
	/// The server which creates new ServerConnections to the proxies.
	Server _server;

	/// The connection between us and the remote switch
	SwitchConnection _switchConnection;

};

#endif

