#ifndef PROXYCONFIG_H_
#define PROXYCONFIG_H_

#include <string>
#include <vector>
#include "QtConfig.h"

/// Manage the configuration for a proxy.
class ProxyConfig : public QtConfig {

public:
	/// Constructor for a specified configuration file path.
	/// @param configFile Path to the configuration file that holds the parameters.
	ProxyConfig(const std::string configFile) throw (std::string);
	/// Constructor for a configuration in the default location.
    /// @param organization The organization.
    /// @param application The application.
	ProxyConfig(const std::string organization, const std::string application) throw (std::string);
	/// destructor
	virtual ~ProxyConfig();

	/// @returns The proxy ID
	std::string id();
    /// @returns The file containing the private key for the proxy to switch SSL link.
    std::string keyFile();
    /// @returns The file containing the private certificate for the proxy to switch SSL link.
	std::string certFile();
	/// @returns The port for communications to the switch
	int proxyPort();
	/// @returns The hostname of the switch
	std::string switchHostName();
	/// @returns The file containing the private certificate for the switch to proxy SSL link.
	std::string switchCertFile();
    /// @returns The instrument definition file
    std::string instrumentFile();

protected:
	/// Common initialization. Read the configuration. Throw an exception
    /// error message if there is a problem with the configuration.
	void init() throw (std::string);

	/// The proxy ID
	std::string _id;
	/// The file containing the private key for the proxy to switch SSL link.
	std::string _keyFile;
	/// The file containing the private certificate for the proxy to switch SSL link.
	std::string _certFile;
	/// The port for communicating to the switch
	int _proxyPort;
	/// The hostname of the switch
	std::string _switchHostName;
	/// The file containing the private certificate for the switch to proxy SSL link.
	std::string _switchCertFile;
	// The instrument definition file.
	std::string _instrumentFile;

};

#endif /* PROXYCONFIG_H_ */
