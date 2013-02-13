#ifndef SWITCHCONFIG_H_
#define SWITCHCONFIG_H_

#include <string>
#include <vector>
#include "QtConfig.h"

/// Manage the configuration for a switch.
class SwitchConfig : public QtConfig {

public:
	/// Constructor for a specified configuration file path.
	/// @param configFile Path to the configuration file that holds the parameters.
	SwitchConfig(const std::string configFile) throw (std::string);
	/// Constructor for a configuration in the default location.
    /// @param organization The organization.
    /// @param application The application.
	SwitchConfig(const std::string organization, const std::string application) throw (std::string);
	/// destructor
	virtual ~SwitchConfig();

	/// @returns The local port for message from the remote switch
	int localPort();
	/// @returns The remote switch IP
	std::string remoteIP();
	/// @returns The destination port for messages to the remote switch
	int remotePort();
	/// @returns The key file for symmetric cipher encryption over SwitchConnection
    std::string cipherKey();
    /// @returns The type of switch (true for an SSL proxy, false for an embedded proxy)
    bool sslProxy();

	// @returns The port for communications to the SslProxy
	int proxyPort();
    /// @returns The file containing the private key for the switch to proxy SSL link.
    std::string serverKeyFile();
    /// @returns The file containing the private certificate for the switch to proxy SSL link.
	std::string serverCertFile();
	/// @returns The proxy definitions
	std::vector<std::map<std::string, std::string> > proxies();

    /// @returns The instrument definition files
    std::vector<std::map<std::string, std::string> > instruments();

protected:
	/// Common initialization. Read the configuration. Throw an exception
    /// error message if there is a problem with the configuration.
	void init() throw (std::string);

	/// Parameters shared by SSL proxy and embedded proxy switches
	//////////////////////////////////////////////////////////////
	/// Listen on this port for messages from the remote switch
	int _localPort;
	/// The remote switch IP
	std::string _remoteIP;
	/// The destination port for messages to the remote switch
	int _remotePort;
	/// The file containing the key for symmetric cipher encryption over
	/// SwitchConnection. This file must be kept private!
	std::string _cipherKey;
	/// If true, this is for an SSL proxy switch. Otherwise, it is
	/// for an embedded proxy switch.
	bool _sslProxy;

	/// Parameters for SSL proxy switch
	///////////////////////////////////
	// The port for communications to the SslProxy
	int _proxyPort;
	/// The file containing the private key for the switch to proxy SSL link.
	std::string _serverKeyFile;
	/// The file containing the private certificate for the switch to proxy SSL link.
	std::string _serverCertFile;
	/// The proxy definitions
	std::vector<std::map<std::string, std::string> > _proxies;

	/// Parameters for embedded proxy switch
	////////////////////////////////////////
	/// Instrument definition files
	std::vector<std::map<std::string, std::string> > _instruments;

};

#endif /* SWITCHCONFIG_H_ */
