#include "ProxyConfig.h"
#include <iostream>
#include <algorithm>
#include <sstream>

/////////////////////////////////////////////////////////////////////
ProxyConfig::ProxyConfig(const std::string configPath) throw (std::string) :
QtConfig(configPath)
{
	init();
}

/////////////////////////////////////////////////////////////////////
ProxyConfig::ProxyConfig(const std::string organization, const std::string application) throw (std::string) :
QtConfig(organization, application)
{
	init();
}

/////////////////////////////////////////////////////////////////////
ProxyConfig::~ProxyConfig()
{
}

/////////////////////////////////////////////////////////////////////
void ProxyConfig::init() throw (std::string)
{
	// Get configuration values. Default values will be created in the
	// configuration file if they don't exist, so that running the program
	// for the first time will create a configuration template.

	_id             = getString("ProxyID",           "PROXY");
	_keyFile        = getString("ProxyKeyFile",      "./proxy.key");
	_certFile       = getString("ProxyCertFile",     "./proxy.crt");
	_proxyPort      = getInt   ("SSLProxyPort",      0);
	_switchHostName = getString("SwitchHostName",    "127.0.0.1");
	_switchCertFile = getString("SwitchSSLCertFile", "./switch.crt");

	// If the port number is 0, it indicates that the user has not configured
	// the application yet. We wait until this point so that all of the default
	// values will have been added to the configuration file. Force them to
	// take a stab at configuration.
	if (_proxyPort == 0) {
		std::string errmsg = "Please create a usable configuration by editing ";
		errmsg += fileName();
		errmsg += " (make sure port is valid)";
		throw (errmsg);
	}

	// Get the instrument definition file. The proxy program will currently
	// only support one instrument. This could be changed if necessary.
	_instrumentFile = getString("InstrumentFile", "");
}

/////////////////////////////////////////////////////////////////////
std::string ProxyConfig::id()
{
	return _id;
}

/////////////////////////////////////////////////////////////////////
std::string ProxyConfig::keyFile()
{
	return _keyFile;
}

/////////////////////////////////////////////////////////////////////
std::string ProxyConfig::certFile()
{
	return _certFile;
}

/////////////////////////////////////////////////////////////////////
int ProxyConfig::proxyPort()
{
	return _proxyPort;
}

/////////////////////////////////////////////////////////////////////
std::string ProxyConfig::switchHostName()
{
	return _switchHostName;
}

/////////////////////////////////////////////////////////////////////
std::string ProxyConfig::switchCertFile()
{
	return _switchCertFile;
}

/////////////////////////////////////////////////////////////////////
std::string ProxyConfig::instrumentFile()
{
	return _instrumentFile;
}
