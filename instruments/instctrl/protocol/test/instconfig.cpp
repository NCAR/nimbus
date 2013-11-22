#include "InstConfig.h"
#include <iostream>
#include <vector>

/// Test the InstConfig class
int main(int argc, char** argv) {

	// Read the proxy configuration
	InstConfig instConfig;

	// Get the instrument information
	std::cout << "Instrument name:             " << instConfig.instrumentName() << std::endl;
	std::cout << "Instrument incoming port:    " << instConfig.instIncomingPort() << std::endl;
	std::cout << "Instrument destination host: " << instConfig.instDestHost() << std::endl;
	std::cout << "Instrument destination port: " << instConfig.instDestPort() << std::endl;
	std::cout << "User incoming port:          " << instConfig.userIncomingPort() << std::endl;
	std::cout << "User destination host:       " << instConfig.userDestHost() << std::endl;
	std::cout << "User destination port:       " << instConfig.userDestPort() << std::endl;

	// Get the message definitions
	std::vector<InstConfig::MessageInfo> messages = instConfig.messages();

	std::cout << "Messages:" << std::endl;
	for (int i = 0; i < messages.size(); i++) {
		std::cout << "  ID:" << messages[i].msgID;
		std::cout << ", Broadcast:" << (messages[i].broadcast? "true":"false");
		std::cout << ", RateLimit:" << messages[i].rateLimit << std::endl;
	}
}
