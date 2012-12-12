#ifndef SWITCH_H_H
#define SWITCH_H_H

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>

#include "EmbeddedProxyServer.h"
#include "SslProxyServer.h"
#include "SwitchConnection.h"
#include "SymCipherProtocol.h"

/// The application which manages communications between one or more
/// instances of Proxy and another Switch.
///
/// There are two types of switch: one which communicates with
/// the proxies via SSL, and the other which embeds local
/// proxies within the Switch.
///
/// A SwitchConection manages the connection between the two switches.
///
/// A SwitchServer manages the connections between the proxies and the switch.
/// There are two flavors of SwitchServer: the SwitchSslServer for handling
/// remote proxies, and the SwitchEmbeddedServer for the local aggregated proxies.
class Switch: public QObject {
	Q_OBJECT
public:
	/// The flavor of Switch for remote proxies that connect via SSL.
	/// @param keyFile An SSL related parameter: the path to the file containing the private key.
	/// @param certFile An SSL related parameter: the path to the file containing the certificate that matched the private key.
	/// @param switchPort An SSL related parameter: the ssl server port number.
	/// @param caDatabase An SSL related parameter: paths to certs that should be added to the CAdatabase. These will be
	/// certificates for all allowable proxies.
	/// @param localPort A SwitchConection related parameter: The port that we listen for incoming messages on.
	/// @param remoteIP A SwitchConection related parameter: The remote IP that we send messages to.
	/// @param remotePort A SwitchConection related parameter: The port that we send messages to.
	/// @param switchCipherKey A SwitchConection related parameter: The file containing the key for symmetric cipher
	/// encryption over a SwitchConnection.
	Switch(
			std::string keyFile,
			std::string certFile,
			int switchPort,
			std::vector<std::string> caDatabase,
			int _localPort,
			std::string _remoteIP,
			int _remotePort,
			std::string switchCipherKey);

	/// The flavor of switch which contains embedded proxies. It does not
	/// provide an SSL server, so none of the SSL certificates and keys are
	/// needed.
	Switch(
			std::vector<std::string> proxySpecs,
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
	/// Perform the signal/slot connections
	void init();

	/// The server that manages the connections to the proxies.
	SwitchServer* _server;

	/// The connection between us and the remote switch
	SwitchConnection _switchConnection;

};

#endif

