#include "InstConfig.h"
#include <iostream>
#include <algorithm>
#include <sstream>

/////////////////////////////////////////////////////////////////////
InstConfig::InstConfig(QtConfig& config):
_config(config)
{

	// If these items don't exist in the configuration,
	// they will be created as default values.
    _instName              = config.getString("InstName",    "INSTRUMENT");
	_incomingPort          = config.getInt   ("InstIncomingPort",0);
	_destIP                = config.getString("InstHostName",    "127.0.0.1");
	_destPort              = config.getInt   ("InstDestPort", 0);

    // There will be one entry for each type of message.
    std::vector<std::map<std::string, std::string> > defaultValues;
    std::map<std::string, std::string> entry;
    entry["ID"] = "MSG_ID";
    entry["Broadcast"] = "false";
    entry["RateLimit"] = "0.0";
    defaultValues.push_back(entry);

    // Create a default message via getArray().
    std::vector<std::map<std::string, std::string> > array;
    array = config.getArray("Messages", defaultValues);
}

/////////////////////////////////////////////////////////////////////
InstConfig::~InstConfig() {

}
/////////////////////////////////////////////////////////////////////
std::string InstConfig::instrumentName() {
	// The default value was set in the constructor
	_instName = _config.getString("InstName","");
	return _instName;
}
/////////////////////////////////////////////////////////////////////
int InstConfig::incomingPort() {
	// The default value was set in the constructor
	_incomingPort = _config.getInt("InstIncomingPort",0);
	return _incomingPort;
}
/////////////////////////////////////////////////////////////////////
std::string InstConfig::destIP() {
	// The default value was set in the constructor
	_destIP = _config.getString("InstHostName", "");
	return _destIP;
}
/////////////////////////////////////////////////////////////////////
int InstConfig::destPort() {
	// The default value was set in the constructor
	_destPort = _config.getInt("InstDestPort", 0);
	return _destPort;
}

std::vector<InstConfig::MessageInfo> InstConfig::messages() {

	std::vector<InstConfig::MessageInfo> result;

	// get the Messages entries
    std::vector<std::map<std::string, std::string> > defaultValues;
    std::vector<std::map<std::string, std::string> > array;
	array = _config.getArray("Messages", defaultValues);

	for (int i = 0; i < array.size(); i++) {
		// Translate the message entry into a MessageInfo structure
		std::map<std::string, std::string> entry = array[i];

		InstConfig::MessageInfo info;
		std::map<std::string, std::string>::iterator j;

		if ((j = entry.find("ID")) != entry.end()) {
			info.msgID = entry["ID"];
		}

		if ((j = entry.find("Broadcast")) != entry.end()) {
			std::string s = entry["Broadcast"];
			std::transform(s.begin(), s.end(), s.begin(), ::toupper);
			info.broadcast = (s.compare("TRUE")? false : true);
		} else {
			info.broadcast = false;
		}

		if ((j = entry.find("RateLimit")) != entry.end()) {
			std::stringstream ss;
			ss << entry["RateLimit"];
			ss >> info.rateLimit;
		} else {
			info.rateLimit = 0.0;
		}

		result.push_back(info);
	}

	return result;
}



