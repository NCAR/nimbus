#include "InstConfig.h"
#include <iostream>
#include <vector>

/// Test the InstConfig class
int main(int argc, char** argv) {

	// Read the proxy configuration
	InstConfig instConfig("Test", "InstConfigTest");

	// Get the instrument information
	std::cout << "Instrument name:  " << instConfig.instrumentName() << std::endl;
	std::cout << "Incoming port:    " << instConfig.incomingPort() << std::endl;
	std::cout << "Destination host:   " << instConfig.destHost() << std::endl;
	std::cout << "Destination port: " << instConfig.destPort() << std::endl;

	// Get the message definitions
	std::vector<InstConfig::MessageInfo> messages = instConfig.messages();

	std::cout << "Messages:" << std::endl;
	for (int i = 0; i < messages.size(); i++) {
		std::cout << "  ID:" << messages[i].msgID;
		std::cout << ", Broadcast:" << (messages[i].broadcast? "true":"false");
		std::cout << ", RateLimit:" << messages[i].rateLimit << std::endl;
	}
}
