#include "InstConfig.h"
#include <iostream>
#include <algorithm>
#include <sstream>

/////////////////////////////////////////////////////////////////////
InstConfig::InstConfig() throw (std::string) {

}

/////////////////////////////////////////////////////////////////////
InstConfig::InstConfig(const std::string configPath) throw (std::string):
_instConfigPath(configPath)
{
	init();
}

/////////////////////////////////////////////////////////////////////
InstConfig::InstConfig(const std::string organization, const std::string application) throw (std::string):
	_instConfigOrg(organization),
	_instConfigApp(application)
{
	init();
}

/////////////////////////////////////////////////////////////////////
InstConfig::~InstConfig() {

}

/////////////////////////////////////////////////////////////////////
void InstConfig::init() throw (std::string) {

	// Get the instrument configuration
	QtConfig* config;
	if (_instConfigPath.size() > 0) {
		// This will create the file if it doesn't already exist. But
		// the user will need to edit it.
		config = new QtConfig(_instConfigPath);
	} else {
		config = new QtConfig(_instConfigOrg, _instConfigApp);
	}

	// If these items don't exist in the configuration,
	// they will be created as default values.
    _instName     = config->getString("InstName",         "INSTRUMENT");
	_incomingPort = config->getInt   ("InstIncomingPort", 0);
	_destHost     = config->getString("InstHostName",     "127.0.0.1");
	_destPort     = config->getInt   ("InstDestPort",     0);
	if (_instName == "INSTRUMENT" || _incomingPort == 0 || _destPort == 0) {
		std::string errmsg = "The instrument configuration file ";
		errmsg += config->fileName();
		errmsg += " contains errors.";
		throw (errmsg);
	}

    // Get the message definitions. Create a default entry in case
	// none are there yet.
    std::vector<std::map<std::string, std::string> > allMsgs;
    std::map<std::string, std::string> entry;
    entry["ID"] = "MSG_ID";
    entry["Broadcast"] = "false";
    entry["RateLimit"] = "0.0";
    allMsgs.push_back(entry);

    // Get the message details from the configuration.
    allMsgs = config->getArray("Messages", allMsgs);

	for (int i = 0; i < allMsgs.size(); i++) {

		// Translate the message entry into a MessageInfo structure
		std::map<std::string, std::string> entry = allMsgs[i];

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

		_messages.push_back(info);
	}

	delete config;

}

/////////////////////////////////////////////////////////////////////
std::string InstConfig::instrumentName() {
	// The default value was set in the constructor
	return _instName;
}

/////////////////////////////////////////////////////////////////////
int InstConfig::incomingPort() {
	return _incomingPort;
}

/////////////////////////////////////////////////////////////////////
std::string InstConfig::destHost() {
	return _destHost;
}

/////////////////////////////////////////////////////////////////////
int InstConfig::destPort() {
	return _destPort;
}

/////////////////////////////////////////////////////////////////////
std::vector<InstConfig::MessageInfo> InstConfig::messages() {
	return _messages;
}
