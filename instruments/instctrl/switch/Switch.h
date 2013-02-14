#ifndef SWITCH_H_H
#define SWITCH_H_H

#include <QtCore>
#include <QtNetwork>
#include <string>
#include <vector>

#include "EmbeddedProxyServer.h"
#include "SslProxyServer.h"
#include "SwitchConfig.h"
#include "SwitchConnection.h"
#include "SwitchMonitor.h"
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
	/// Constructor
	/// @param config The switch configuration
	/// @param verbose If true print messages passing through the switch.
	/// @param reportPeriodSecs The system log reporting period, in secods.
	Switch(SwitchConfig* config, bool verbose, int reportPeriodSecs = 60);
	/// Destructor
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
	SwitchConnection* _switchConnection;
	/// The switch monitor.
	SwitchMonitor* _switchMonitor;
	/// The logging facility
	RicLogger _logger;
	/// Ture to print message passing through the switch
	bool _verbose;
	/// The total number of messages to switch .
	int _msgsToSwitch;
	/// The total number of messages from switch.
	int _msgsFromSwitch;
	/// The total number of messages to proxy.
	int _msgsToProxies;
	/// The total number of messages from proxy.
	int _msgsFromProxies;

};

#endif

