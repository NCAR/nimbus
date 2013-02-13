#include "SwitchConfig.h"
#include <iostream>
#include <algorithm>
#include <sstream>

/////////////////////////////////////////////////////////////////////
SwitchConfig::SwitchConfig(const std::string configPath) throw (std::string) :
QtConfig(configPath)
{
	init();
}

/////////////////////////////////////////////////////////////////////
SwitchConfig::SwitchConfig(const std::string organization, const std::string application) throw (std::string) :
QtConfig(organization, application)
{
	init();
}

/////////////////////////////////////////////////////////////////////
SwitchConfig::~SwitchConfig()
{
}

/////////////////////////////////////////////////////////////////////
void SwitchConfig::init() throw (std::string)
{
	// Get configuration values. Default values will be created in the
	// configuration file if they don't exist, so that running the program
	// for the first time will create a configuration template.

	// Get parameters shared by SSL proxy switch and embedded proxy switch
	_localPort  = getInt   ("SwitchLocalPort",  0);
	_remoteIP   = getString("SwitchRemoteIP",   "127.0.0.1");
	_remotePort = getInt   ("SwitchRemotePort", 0);
	_cipherKey  = getString("SwitchCipherKey",  "./udpcipher.key");
	_sslProxy   = getBool  ("SslProxy",         true);

	// If the port number is 0 or no IP address, it indicates that the user
	// has not configured the application yet. We wait until this point so
	// that all of the default values will have been added to the configuration
	// file. Force them to take a stab at configuration.
	if (_localPort == 0 || _remotePort == 0 || _remoteIP.empty()) {
		std::string errmsg = "SwitchConfig: Please create a usable configuration by editing ";
		errmsg += fileName();
		errmsg += " (make sure ports and IP addresses are valid)";
		throw (errmsg);
	}

	if (_sslProxy) {
		// Get parameters for SSL proxy switch
		_proxyPort      = getInt   ("SSLProxyPort",      0);
		_serverKeyFile  = getString("SwitchSSLKeyFile",  "./switch.key");
		_serverCertFile = getString("SwitchSSLCertFile", "./switch.crt");

		// Get the proxy definitions
		_proxies = getArray("SSLProxies", _proxies);
	} else {
		// Get the instrument definition files for embedded proxy switch
		_instruments = getArray("Instruments", _instruments);
	}
}

/////////////////////////////////////////////////////////////////////
int SwitchConfig::localPort()
{
	return _localPort;
}

/////////////////////////////////////////////////////////////////////
std::string SwitchConfig::remoteIP()
{
	return _remoteIP;
}

/////////////////////////////////////////////////////////////////////
int SwitchConfig::remotePort()
{
	return _remotePort;
}

/////////////////////////////////////////////////////////////////////
std::string SwitchConfig::cipherKey()
{
	return _cipherKey;
}

/////////////////////////////////////////////////////////////////////
bool SwitchConfig::sslProxy()
{
	return _sslProxy;
}

/////////////////////////////////////////////////////////////////////
int SwitchConfig::proxyPort()
{
	return _proxyPort;
}

/////////////////////////////////////////////////////////////////////
std::string SwitchConfig::serverKeyFile()
{
	return _serverKeyFile;
}

/////////////////////////////////////////////////////////////////////
std::string SwitchConfig::serverCertFile()
{
	return _serverCertFile;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::map<std::string, std::string> > SwitchConfig::proxies()
{
	return _proxies;
}

/////////////////////////////////////////////////////////////////////
std::vector<std::map<std::string, std::string> > SwitchConfig::instruments()
{
	return _instruments;
}
