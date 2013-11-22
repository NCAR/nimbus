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

	_sslServer      = getBool  ("SslServer",         true);
	_topDir         = getString("TopDir"   ,       "~/.ric");
	_localPort      = getInt   ("SwitchLocalPort",  0);
	_remoteIP       = getString("SwitchRemoteIP",   "127.0.0.1");
	_remotePort     = getInt   ("SwitchRemotePort", 0);
	_cipherKeyFile  = getString("CipherKey",  "Cipher");
	_logInterval    = getInt   ("LogInterval",      300);

	// Change tilde to home path
	_topDir = expandPath(_topDir);

	// Convert to complete file path.
	_cipherKeyFile  = _topDir + "/Certs/" + _cipherKeyFile + ".key";

	if (_sslServer) {
		// Get parameters for SSL proxy switch
		_proxyPort      =    getInt("SSLPort", 0);
		_serverKeyFile  = getString("SSLKey",  "Localhost");
		_serverCertFile = getString("SSLCert", "Localhost");
		// Convert to complete file path.
		_serverKeyFile  = _topDir + "/Certs/" + _serverKeyFile  + ".key";
		_serverCertFile = _topDir + "/Certs/" + _serverCertFile + ".crt";
	}

	// Get the instrument definitions. These will provide:
	// - The instrument name (used to create the SSL cert file name for SSLProxy switches).
	// - The network parameters  for both SSLProxy and EmbeddedProxy switches.
	// - The message definitions for both SSLProxy and EmbeddedProxy switches.
	_instruments = getArray("Instruments", _instruments);
	// Convert the file names to absolute paths
	for (std::vector<std::map<std::string,std::string> >::iterator i = _instruments.begin();
			i != _instruments.end(); i++) {
		if ((*i).find("InstrumentFile") != (*i).end()) {
			(*i)["InstrumentFile"] = _topDir + "/Instruments/" + (*i)["InstrumentFile"];
		}
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

	if (!fileReadable(_cipherKeyFile, "CipherKey file", e)) {
		msg += msg.size()?"\n":"";
		msg += e;
		valid = false;
	}

	if (_sslServer) {
		if (_proxyPort == 0 ) {
			msg += msg.size()?"\n":"";
			msg += "SSLProxyPort must be non-zero. ";
			valid = false;
		}
		/// @todo Add check for user readonly.
		if (!fileReadable(_serverKeyFile, "SSLKey file", e)) {
			msg += msg.size()?"\n":"";
			msg += e;
			valid = false;
		}
		if (!fileReadable(_serverCertFile, "SSLCert file", e)) {
			msg += msg.size()?"\n":"";
			msg += e;
			valid = false;
		}
	}

	// Check instruments. Also populate _proxySpecs
	for (std::vector<std::map<std::string, std::string> >::iterator i = _instruments.begin();
			i != _instruments.end(); i++) {

		// Was an InstrumentFile specified?
		if (i->find("InstrumentFile")==i->end()) {
			msg += msg.size()?"\n":"";
			msg += "InstrumentFile not specified in [Instruments]";
			valid = false;
			break;
		}
		// Is the instrument file accessible?
		std::string instFile = (*i)["InstrumentFile"];
		if (!fileReadable(instFile, "Instrument file", e)) {
			msg += msg.size()?"\n":"";
			msg += e;
			valid = false;
		}

		// add the instrument file to the proxy spec.
		std::map<std::string, std::string> proxySpec;
		proxySpec["InstrumentFile"] = instFile;

		if (_sslServer) {
			// If an SSL server, check for instrument certificates.
			// Parse the instrument file
			InstConfig instConfig(instFile);
			std::string instName = instConfig.instrumentName();
			std::string instCertFile = _topDir + "/Certs/" + instName + ".crt";
			if (!fileReadable(instFile, "Instrument certificate file", e)) {
				msg += msg.size()?"\n":"";
				msg += e;
				valid = false;
			}
			// Add the SSL certificate file to the proxy spec.
			proxySpec["SSLCertFile"] = instCertFile;
		}

		// Save the proxy spec.
		_proxies.push_back(proxySpec);
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
	return _cipherKeyFile;
}

/////////////////////////////////////////////////////////////////////
bool SwitchConfig::sslProxy()
{
	return _sslServer;
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
/////////////////////////////////////////////////////////////////////
std::string SwitchConfig::expandPath(std::string path) {

	std::string result = path;

	std::string home(getenv("HOME"));

	size_t i = 0;
	while (1) {
		i = result.find("~", i);
		if (i == std::string::npos) {
			break;
		}
		result.replace(i, 1, home);
	}

	return result;

}
