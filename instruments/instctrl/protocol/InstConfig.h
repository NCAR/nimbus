#ifndef INSTCONFIG_H_
#define INSTCONFIG_H_

#include <string>
#include <vector>
#include <QtCore>
#include "QtConfig.h"

/// Manage the configuration for instrument/user message handling.
/// This configuration is used by proxies and switches. Some of the
/// configuration parameters are not applicable to a particular application.
/// For instance, the broadcast, IP and port parameters are not used
/// by the switches. Likewise, the rate limit parameter is only used by
/// the switches and not the proxies. The idea is that one configuration
/// file can (mostly) be shared by all applications.
/// @todo Should be derived from QtConfig?
class InstConfig {

public:
	/// A helper class for holding a description of a single message type.
	struct MessageInfo {
		/// The message identifier. Messages must be self-identifying, and
		/// the id extracted from the message will be compared against this.
		std::string msgID;
		/// Set true if outgoing copies of this message should be broadcast.
		/// False for unicast.
		bool broadcast;
		/// The rate limit in seconds. The message rate limiter will only
		/// allow one message through within this time period. Set to 0
		/// for no rate limiting.
		double rateLimit;
	};

	/// Default constructor.
	InstConfig() throw (std::string);
	/// Constructor for a specified configuration file path.
	/// @param configFile Path to the configuration file that holds the parameters.
	InstConfig(const std::string configFile) throw (std::string);
	/// destructor
	virtual ~InstConfig();
	/// Load the configuration.
	void loadConfig();
	/// Make sure that the configuration is somewhat sane. Provide
	/// some diagnostics for detected errors.
	/// @param errMsg The error diagnostics are returned here.
	/// @return True if it passes muster, false otherwise.
	bool validate(std::string& errMsg);
	/// See if the specified file is readable. If not,
	/// return an error message. The message will have the
	/// prefix prepended.
	/// @param path The file path.
	/// @param prefix The error message prefix.
	/// @param errMsg The error message.
	/// @return True if the file is readable, false otherwise.
	bool fileReadable(std::string path,
			std::string prefix, std::string& errMsg);
	/// @returns The port number that will be read for incoming message datagrams
	/// from the instrument.
	int instIncomingPort();
	/// @returns The instrument port number that outgoing message datagrams will be sent to.
	int instDestPort();
	/// @returns The IP name or address of the instrument for outgoing message datagrams.
	std::string instDestHost();
	/// @returns The port number that will be read for incoming message datagrams
	/// from the user.
	int userIncomingPort();
	/// @returns The user port number that outgoing message datagrams will be sent to.
	int userDestPort();
	/// @returns The IP name or address of the user for outgoing message datagrams.
	std::string userDestHost();
	/// @returns The instrument name.
    std::string instrumentName();
    /// @returns The collection of defined messages for this instrument.
    std::vector<MessageInfo> messages();

protected:
	/// Common initialization. Read the configuration. Throw an exception
    /// error message if there is a problem with the configuration.
	void init() throw (std::string);
    /// The instrument name.
    std::string _instName;
    /// The port number that will be read for incoming message datagrams
	/// from the instrument.
	int _instIncomingPort;
	/// The IP name or address of the instrument for outgoing message datagrams.
	std::string _instDestHost;
	/// The instrument port number that outgoing message datagrams will be sent to.
	int _instDestPort;
    /// The port number that will be read for incoming message datagrams
	/// from the user.
	int _userIncomingPort;
	/// The IP name or address of the user for outgoing message datagrams.
	std::string _userDestHost;
	/// The user port number that outgoing message datagrams will be sent to.
	int _userDestPort;
	/// Our messages
	std::vector<MessageInfo> _messages;
	/// The path to the instrument configuration.
	std::string _fileName;
};

#endif /* INSTCONFIG_H_ */
