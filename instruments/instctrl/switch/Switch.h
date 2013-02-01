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
#include "RicLogger.h"

/// The application which manages communications between one or more
/// instances of a proxy and another Switch.
///
/// There are two types of switch: one which communicates with
/// the proxies via SSL, and the other which embeds local
/// proxies within the Switch.
///
/// A SwitchConection manages the connection between the two switches.
///
/// All messages which are transferred between the proxies and the switch
/// connection come through the Switch, so that rate limiting and
/// message filtering can be centrally managed. This is handled in
/// msgFromProxySlot() and msgFromRemoteSwitch().
///
/// Within the switch is a ProxyServer, which manages the connections between the
/// proxies and the switch. It is a server because it can dynamically
/// create connections for proxies. There are two flavors of ProxyServer: the
/// SslProxyServer for handling remote SSL proxies, and the EmbeddedProxyServer
/// for the local embedded proxies.
class Switch: public QObject {
	Q_OBJECT
public:
	/// The flavor of Switch for remote proxies that connect via SSL.
	/// @param serverSslKeyFile An SSL related parameter: the path to the file containing the SSL private key.
	/// @param serverSslCert An SSL related parameter: the SSL certificate that matches the private key.
	/// @param switchPort An SSL related parameter: the ssl server port number.
	/// @param proxies The authorized SSL proxies.
	/// @param localPort A SwitchConection related parameter: The port that we listen for incoming messages on.
	/// @param remoteIP A SwitchConection related parameter: The remote IP that we send messages to.
	/// @param remotePort A SwitchConection related parameter: The port that we send messages to.
	/// @param switchCipherKey A SwitchConection related parameter: The file containing the key for symmetric cipher
	/// encryption over a SwitchConnection.
	/// @param verbose If true print messages passing through the switch
	Switch(
			std::string serverSslKeyFile,
			QSslCertificate serverSslCert,
			int switchPort,
			std::vector<SslProxyServer::SslProxyDef> proxies,
			int _localPort,
			std::string _remoteIP,
			int _remotePort,
			std::string switchCipherKey,
			bool verbose);

	/// The flavor of switch which contains embedded proxies. It does not
	/// provide an SSL server, so none of the SSL certificates and keys are
	/// needed. A proxy will be created for each instrument configuration.
	/// @param instConfigs A list of instrument configurations.
	/// @param localPort A SwitchConection related parameter: The port that we listen for incoming messages on.
	/// @param remoteIP A SwitchConection related parameter: The remote IP that we send messages to.
	/// @param remotePort A SwitchConection related parameter: The port that we send messages to.
	/// @param switchCipherKey A SwitchConection related parameter: The file containing the key for symmetric cipher
	/// encryption over a SwitchConnection.
	/// @param verbose If true print messages passing through the switch
	Switch(
			std::vector<InstConfig> instConfigs,
			int _localPort,
			std::string _remoteIP,
			int _remotePort,
			std::string switchCipherKey,
			bool verbose);

	virtual ~Switch();
	
protected slots:
	/// Handle a new message from a proxy.
	/// @param The incoming message from the proxy.
	void msgFromProxySlot(Protocols::Message msg);
	/// Handle a new message from the remote switch.
	/// @param The incoming message from the remote switch.
	void msgFromRemoteSwitch(Protocols::Message msg);

protected:
	/// Create the signal/slot connections.
	void init();

	/// The server that manages the connections to the proxies.
	ProxyServer* _server;

	/// The connection between us and the remote switch
	SwitchConnection _switchConnection;

	/// Ture to print message passing through the switch
	bool _verbose;

	/// The loggin facility
	RicLogger _logger;

};

#endif

