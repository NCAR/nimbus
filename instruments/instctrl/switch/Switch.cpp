#include "Switch.h"
#include <sstream>

/////////////////////////////////////////////////////////////////////
Switch::Switch(SwitchConfig* config, bool verbose, int reportPeriodSecs) :
_verbose(verbose),
_msgsFromProxies(0),
_msgsToSwitch(0),
_msgsFromSwitch(0),
_msgsToProxies(0),
_msgsFromProxiesDropped(0)
{

	// Create the SwitchMonitor
	_switchMonitor = new SwitchMonitor(reportPeriodSecs,
									   _msgsFromProxies,
									   _msgsToSwitch,
									   _msgsFromSwitch,
									   _msgsToProxies,
									   _msgsFromProxiesDropped);

	// The SslProxy parameter in the configuration determines the type
	// of switch to be created. If SslProxy is true, an SSL proxy switch
	// is created. Otherwise, an embedded proxy switch is created.
	_SslProxy = config->sslProxy();

	// Create the SwichConnection between this switch and the remote switch
	_switchConnection = new SwitchConnection(config->localPort(),
		          	  	  	  	  	  	     config->remoteIP(),
		          	  	  	  	  	  	     config->remotePort(),
		          	  	  	  	  	  	     config->cipherKey());

	if (_SslProxy) {
		/// The flavor of Switch for remote proxies that connect via SSL.

		// The file containing the private key for the switch to proxy SSL link.
		// This file must be kept private!
		std::string serverKey(config->serverKeyFile());

		// The file containing the private certificate for the switch to proxy SSL link.
		std::string serverCertFile(config->serverCertFile());

		QSslCertificate serverCert(QSslCertificate::fromPath(serverCertFile.c_str())[0]);
		if (logCertError(serverCert, serverCertFile)) {
			exit(1);
		}

		// Get the proxy specifications
		std::vector<std::map<std::string, std::string> > proxySpecs;
		proxySpecs = config->proxies();

		std::vector<SslProxyServer::SslProxyDef> proxies;
		for (int i = 0; i < proxySpecs.size(); i++) {

			// Process the next specified SSL proxy certificate
			std::string proxyCertFile = proxySpecs[i]["SSLCertFile"];
			QSslCertificate cert(QSslCertificate::fromPath(proxyCertFile.c_str())[0]);

			// Check for a certificate error, and if no error, create the proxy definition.
			if (!logCertError(cert, proxyCertFile)) {
				// Create a proxy definition
				SslProxyServer::SslProxyDef proxy;
				proxy._sslCert = cert;
				proxy._instConfig = InstConfig(proxySpecs[i]["InstrumentFile"]);
				proxies.push_back(proxy);
			} else {
				exit(1);
			}
		}

		// Create the SSL based switch. The switch creates:
		//  - an SslServer
		//  - a SwitchConnection to the remote switch
		//  - SslServer creates connections to Proxies.
		// Create an SSL proxy server
		_server = new SslProxyServer(serverKey,
									 serverCert,
									 config->proxyPort(),
									 proxies);

		_logger.log("SSL switch was initialized");
	}
	else {
		/// The flavor of switch which contains embedded proxies. It does not
		/// provide an SSL server, so none of the SSL certificates and keys are
		/// needed. A proxy will be created for each instrument configuration.

		// Get the instrument definition files
		std::vector<std::map<std::string, std::string> > instruments;
		instruments = config->instruments();

		// Create the instrument configurations
		std::vector<InstConfig> instConfigs;
		for (int i = 0; i < instruments.size(); i++) {
			/// @todo Need error checking for missing map key
			std::string fileName = instruments[i]["InstrumentFile"];
			instConfigs.push_back(InstConfig(fileName));
		}

		// Create an embedded proxy server
		_server = new EmbeddedProxyServer(instConfigs);

		_logger.log("EmbeddedProxy switch was initialized");
	}

	// The server emits all proxy messages. We will capture them.
	connect(_server, SIGNAL(msgFromProxy(Protocols::Message)),
			this, SLOT(msgFromProxySlot(Protocols::Message)));

	// Capture the messages from the remote switch
	connect(_switchConnection, SIGNAL(msgFromRemoteSwitch(Protocols::Message)),
			this, SLOT(msgFromRemoteSwitch(Protocols::Message)));

	// Set up the switch rate limiter
	setRateLimiter(config);

	// Send a heartbeat and start switch heartbeat timer
	sendSysMsg("HEARTBEAT");
	_heartbeat = startTimer(reportPeriodSecs * 1000);
	
	// Log a few details about the switch to switch connection
	QString msg;
	msg = QString("Inter-switch messages received on port %1").arg(config->localPort());
	_logger.log(msg.toStdString());
	msg = QString("Inter-switch messages sent to %1:%2").arg(config->remoteIP().c_str()).arg(config->remotePort());
	_logger.log(msg.toStdString());
}

/////////////////////////////////////////////////////////////////////
Switch::~Switch()
{
	delete _server;
	delete _switchConnection;
	delete _rateLimiter;
	delete _switchMonitor;

	killTimer(_heartbeat);
}

/////////////////////////////////////////////////////////////////////
void Switch::timerEvent(QTimerEvent* event)
{
	if (event->timerId() == _heartbeat)
		sendSysMsg("HEARTBEAT");
}

/////////////////////////////////////////////////////////////////////
void Switch::setRateLimiter(SwitchConfig* config)
{
	// Create the rate limiter
	_rateLimiter = new RateLimiter();

	// Add all message types passing through the switch to rate limiter
	if (_SslProxy) {
		std::vector<std::map<std::string, std::string> > proxiesConfig;
		proxiesConfig = config->proxies();

		for (int i = 0; i < proxiesConfig.size(); i++) {
			InstConfig instConfig(proxiesConfig[i]["InstrumentFile"]);
			std::vector<InstConfig::MessageInfo> msgs = instConfig.messages();
			for (int j = 0; j < msgs.size(); j++) {
				_rateLimiter->addMsgType(msgs[j].msgID, msgs[j].rateLimit);
			}
		}
	} else {
		std::vector<std::map<std::string, std::string> > instruments;
		instruments = config->instruments();

		for (int i = 0; i < instruments.size(); i++) {
			InstConfig instConfig(instruments[i]["InstrumentFile"]);
			std::vector<InstConfig::MessageInfo> msgs = instConfig.messages();
			for (int j = 0; j < msgs.size(); j++) {
				_rateLimiter->addMsgType(msgs[j].msgID, msgs[j].rateLimit);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////
void Switch::sendSysMsg(std::string msgId, std::string text)
{
	// send a SYS message to the remote switch
	_msgsToSwitch++;
	Protocols::Message message(Protocols::Message::SYS, "", msgId, text);
	std::string msg = message.toJsonStdString();

	try {
		_switchConnection->sendSwitchMessage(msg);
	}
	catch (std::string& emsg) {
		_logger.log("Error sending SYS message to the remote switch:");
		_logger.log(emsg);
	}
}

/////////////////////////////////////////////////////////////////////
void Switch::msgFromProxySlot(Protocols::Message message)
{
	// A message has been received from the proxy
	if (_verbose) {
		std::cout << message.toJsonStdString() << std::endl;
	}

	// The SSL proxy switch will send a HEARTBEAT message
	// to the remote switch when receiving a SYS message
	// (i.e REQUEST) from the proxy.
	if (_SslProxy && message.msgType() == Protocols::Message::SYS) {
		sendSysMsg("HEARTBEAT");
		// Do not forward the REQUEST message to the remote switch
		return;
	}

	_msgsFromProxies++;

	if (_rateLimiter->checkLimit(message)) {
		//std::cout << "Message " << message.msgId() << " forwarded." << std::endl;
		// The message is OK to be forwarded to the switch
		_msgsToSwitch++;

		// send the proxy message to the remote switch
		std::string msg = message.toJsonStdString();
		try {
			_switchConnection->sendSwitchMessage(msg);
		}
		catch (std::string& emsg) {
			_logger.log("Error sending proxy message to the remote switch:");
			_logger.log(emsg);
		}
	}
	else {
		//std::cout << "Message " << message.msgId() << " dropped." << std::endl;
		_msgsFromProxiesDropped++;
	}
}

/////////////////////////////////////////////////////////////////////
void Switch::msgFromRemoteSwitch(Protocols::Message message)
{
	// A message has been received from the remote switch
	if (_verbose) {
		std::cout << message.toJsonStdString() << std::endl;
	}

	// The embedded proxy switch will send a RESPONSE message
	// to the remote switch when receiving a SYS message (i.e
	// HEARTBEAT) from the remote switch.
	if (!_SslProxy && message.msgType() == Protocols::Message::SYS)
		sendSysMsg("RESPONSE");

	_msgsFromSwitch++;
	_msgsToProxies++;

	// Tell the server to forward the message to the proxy
	_server->sendToProxy(message);
}

/////////////////////////////////////////////////////////////////////
bool Switch::logCertError(QSslCertificate& cert, std::string& path) {

	// Build the error message here.
	std::string msg;

	if (!cert.isValid()) {
			msg += "Invalid certificate: " + path + ",\nvalid "
					+ cert.effectiveDate().toString().toStdString() + " to "
					+ cert.expiryDate().toString().toStdString();
	}
	if (cert.isNull()) {
			msg += "Null certificate specified in " + path;
	}

	if (msg.size()) {
		_logger.log(msg);
		return true;
	}

	return false;
}
