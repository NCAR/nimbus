#include "SwitchConfig.h"
#include <iostream>
#include <algorithm>
#include <sstream>

/////////////////////////////////////////////////////////////////////
SwitchConfig::SwitchConfig() throw (std::string) :
QtConfig("NCAR", "Switch")
{
}

/////////////////////////////////////////////////////////////////////
SwitchConfig::SwitchConfig(const std::string configPath) throw (std::string) :
QtConfig(configPath)
{
	init();
}

/////////////////////////////////////////////////////////////////////
SwitchConfig::~SwitchConfig()
{
	init();
}

/////////////////////////////////////////////////////////////////////
void SwitchConfig::init() throw (std::string)
{
	_localPort  = getInt   ("SwitchLocalPort",  0);
	_remoteIP   = getString("SwitchRemoteIP",   "127.0.0.1");
	_remotePort = getInt   ("SwitchRemotePort", 0);
	_cipherKey  = getString("SwitchCipherKey",  "./udpcipher.key");
	_sslSwitch  = getBool  ("SslProxy",         true);

	// If the port number is 0, it indicates that the user has not configured
	// the application yet. We wait until this point so that all of the default values
	// will have been added to the configuration file. Force them to take a stab at
	// configuration.
	if (_localPort == 0 || _remotePort == 0) {
		std::string errmsg = "Please create a usable configuration by editing ";
		errmsg += fileName();
		errmsg += " (make sure ports and IP addresses are valid)";
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
bool SwitchConfig::sslSwitch()
{
	return _sslSwitch;
}
