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
#include "RateLimiter.h"
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
	
	virtual void timerEvent(QTimerEvent* event);

protected slots:
	/// Handle a new message from a proxy.
	/// @param msg The incoming message from the proxy.
	void msgFromProxySlot(Protocols::Message msg);
	/// Handle a new message from the remote switch.
	/// @param msg The incoming message from the remote switch.
	void msgFromRemoteSwitch(Protocols::Message msg);

protected:
	/// Create the signal/slot connections.
	void init();
	/// Set up rate limiter for the switch
	void setRateLimiter(SwitchConfig* config);
	/// Send a SYS message to remote switch
	/// @param msgId The SYS message ID
	/// @param text The SYS message content
	void sendSysMsg(std::string msgId, std::string text="");
	/// The server that manages the connections to the proxies.
	ProxyServer* _server;
	/// The connection between us and the remote switch
	SwitchConnection* _switchConnection;
	/// The switch monitor.
	SwitchMonitor* _switchMonitor;
	/// The rate limiter
	RateLimiter* _rateLimiter;
	/// The logging facility
	RicLogger _logger;
	/// If if the switch is an SSL proxy switch, otherwise the
	/// switch is an embedded proxy switch.
	bool _SslProxy;
	/// Ture to print message passing through the switch
	bool _verbose;
	/// The total number of messages to remote switch.
	int _msgsToSwitch;
	/// The total number of messages from remote switch.
	int _msgsFromSwitch;
	/// The total number of messages to proxies.
	int _msgsToProxies;
	/// The total number of messages from proxies.
	int _msgsFromProxies;
	/// The total number of messages from proxies that got dropped
	int _msgsFromProxiesDropped;
	/// The heartbeat timer
	int _heartbeat;

};

#endif

