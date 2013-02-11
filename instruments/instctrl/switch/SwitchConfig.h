#ifndef SWITCHCONFIG_H_
#define SWITCHCONFIG_H_

#include <string>
#include <vector>
#include <QtCore>
#include "QtConfig.h"

/// Manage the configuration for a switch.
class SwitchConfig : public QtConfig {

public:
	/// Default constructor (for a configuration in the default location).
	SwitchConfig() throw (std::string);
	/// Constructor for a specified configuration file path.
	/// @param configFile Path to the configuration file that holds the parameters.
	SwitchConfig(const std::string configFile) throw (std::string);
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
    bool sslSwitch();

protected:
	/// Common initialization. Read the configuration. Throw an exception
    /// error message if there is a problem with the configuration.
	void init() throw (std::string);
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
	bool _sslSwitch;

};

#endif /* SWITCHCONFIG_H_ */
