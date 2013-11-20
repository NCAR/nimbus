#include "SwitchConfig.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <QtCore>

/////////////////////////////////////////////////////////////////////
SwitchConfig::SwitchConfig(const std::string configPath) throw (std::string):
QtConfig(configPath)
{
	init();
}

/////////////////////////////////////////////////////////////////////
SwitchConfig::SwitchConfig() throw (std::string):
QtConfig(QString(QDir::homePath()+"/.ric/Switch.ini").toStdString())
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
	// Load the configuration
	loadConfig();

	// Validate the configuration.
	std::string e;
	if (!validate(e)) {
		std::string errmsg = "Errors found in the configuration(";
		errmsg += fileName();
		errmsg += "):\n";
		errmsg += e;
		throw (errmsg);
	}
}
/////////////////////////////////////////////////////////////////////
void SwitchConfig::loadConfig() {
	// Get configuration values. Default values will be created in the
	// configuration file if they don't exist, so that running the program
	// for the first time will create a configuration template.

	// Get parameters shared by SSL proxy switch and embedded proxy switch
	_localPort  = getInt   ("SwitchLocalPort",  0);
	_remoteIP   = getString("SwitchRemoteIP",   "127.0.0.1");
	_remotePort = getInt   ("SwitchRemotePort", 0);
	_cipherKey  = getString("SwitchCipherKey",  "./udpcipher.key");
	_sslProxy   = getBool  ("SslProxy",         true);
	_logInterval= getInt   ("LogInterval",      300);

	if (_sslProxy) {
		// Get parameters for SSL proxy switch
		_proxyPort      = getInt   ("SSLProxyPort",      0);
		_serverKeyFile  = getString("SwitchSSLKeyFile",  "~/.ric/SwitchCerts/switch.key");
		_serverCertFile = getString("SwitchSSLCertFile", "~/.ric/SwitchCerts/switch.crt");

		// Get the proxy definitions
		_proxies = getArray("SSLProxies", _proxies);
	} else {
		// Get the instrument definition files for an embedded proxy switch
		_instruments = getArray("Instruments", _instruments);
	}
}
/////////////////////////////////////////////////////////////////////
bool SwitchConfig::validate(std::string& errMsg) {

	bool valid = true;
	std::string msg;
	std::string e;

	if (_localPort == 0 ) {
		msg += "SwitchLocalPort must be non-zero. ";
		valid = false;
	}
	if ( _remotePort == 0) {
		msg += msg.size()?"\n":"";
		msg += "SwitchRemotePort must be non-zero. ";
		valid = false;
	}

	if (!fileReadable(_cipherKey, "SwitchCipherKey", e)) {
		msg += msg.size()?"\n":"";
		msg += e;
		valid = false;
	}

	if (_sslProxy) {
		if (_proxyPort == 0 ) {
			msg += msg.size()?"\n":"";
			msg += "SSLProxyPort must be non-zero. ";
			valid = false;
		}
		/// @todo Add check for user readonly.
		if (!fileReadable(_serverKeyFile, "SwitchSSLKeyFile", e)) {
			msg += msg.size()?"\n":"";
			msg += e;
			valid = false;
		}
		if (!fileReadable(_serverCertFile, "SwitchSSLCertFile", e)) {
			msg += msg.size()?"\n":"";
			msg += e;
			valid = false;
		}
		// Check proxies
		for (std::vector<std::map<std::string, std::string> >::iterator i = _proxies.begin();
				i != _proxies.end(); i++) {
			if (i->find("InstrumentFile")==i->end()) {
				msg += msg.size()?"\n":"";
				msg += "InstrumentFile not specified for SSLProxy";
				valid = false;
				break;
			}
			std::string proxyName = (*i)["InstrumentFile"];
			if (i->find("SSLCertFile")==i->end()) {
				msg += msg.size()?"\n":"";
				msg += "SSLCertFile not specified for SSLProxy " + proxyName;
				valid = false;
			} else {
				if (!fileReadable((*i)["SSLCertFile"], "SSLCertFile for proxy " + proxyName, e)) {
					msg += msg.size()?"\n":"";
					msg += e;
					valid = false;
				}
			}
		}
		// Check instruments
		//_instruments
	} else {
		// Check instruments
		//_instruments
	}

	// Set the error messages.
	errMsg = msg;

	return valid;
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

/////////////////////////////////////////////////////////////////////
int SwitchConfig::logInterval()
{
	return _logInterval;
}

/////////////////////////////////////////////////////////////////////
bool SwitchConfig::fileReadable(std::string path,
		std::string prefix, std::string& errMsg) {

	QFileInfo fileInfo(path.c_str());

	if (!fileInfo.isReadable()) {
		errMsg = prefix + "(" + path
				+ ") does not specify an accessible file. ";
		return false;
	}
	return true;
}
