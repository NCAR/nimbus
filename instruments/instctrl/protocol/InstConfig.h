#ifndef PROXYCONFIG_H_
#define PROXYCONFIG_H_

#include <string>
#include <vector>
#include <QtCore>
#include "QtConfig.h"

/// Manage the configuration for instrument/user message handling.
/// This configuration is used by proxies and switches. Some of the
/// configuration parameters are not applicable to a particular application. For
/// instance, the broadcast, IP and port parameters are not used
/// by the switches. Likewise, the rate limit parameter is only used by
/// the switches and not the proxies. The idea is that one configuration
/// file can (mostly) be shared by all applications.
/// @todo Should be derived from QtConfig?
class InstConfig {
public:
	/// A helper class for holding a description of a single message type.
	struct MessageInfo {
		/// The message identifier. Messages must be self-identifying, and
		/// the id extracted from the message will be compared agains this.
		std::string msgID;
		/// Set true if outgoing copies of this message should be broadcast.
		/// Flase for unicast.
		bool broadcast;
		/// The rate limit in seconds. The message rate limiter will only
		/// allow one message through within this time period. Set to 0
		/// for no rate limiting.
		double rateLimit;
	};
public:
	/// Default constructor.
	InstConfig() throw (std::string);
	/// Constructor for a specified configuration file path.
	/// @param configFile Path to the configuration file that holds the parameters.
	InstConfig(const std::string configFile) throw (std::string);
	/// Constructor for a configuration in the default location.
    /// @param organization The organization.
    /// @param application The application.
	InstConfig(const std::string organization, const std::string application) throw (std::string);
	/// destructor
	virtual ~InstConfig();
	/// @returns The port number that will be read for incoming message datagrams
	/// from the user or instrument.
	int incomingPort();
	/// @returns The port number that outgoing message datagrams will be sent to.
	int destPort();
	/// @returns The IP name or address of the destination for outgoing message datagrams.
	std::string destHost();
	/// @returns The instrument name.
    std::string instrumentName();
    /// @returns The collection of defined messagfes for this instrument.
    std::vector<MessageInfo> messages();

protected:
	/// Common initialization. Read the configuration.
	void init();
    /// The instrument name.
    std::string _instName;
    /// The port number that will be read for incoming message datagrams
	/// from the user or instrument.
	int _incomingPort;
	/// The IP name or address of the destination for outgoing message datagrams.
	std::string _destHost;
	/// The port number that outgoing message datagrams will be sent to.
	int _destPort;
	/// Our messages
	std::vector<MessageInfo> _messages;
	/// The path to the instrument configuration. If blank,
	/// The standard location will be used with organization, application
	std::string _instConfigPath;
	/// Use for configuration organization when path is not provided.
	std::string _instConfigOrg;
	/// Use for configuration application when path is not provided
	std::string _instConfigApp;

};


#endif /* PROXYCONFIG_H_ */
