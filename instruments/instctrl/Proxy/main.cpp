#include <iostream>
#include <string>
#include <sstream>
#include <boost/program_options.hpp>
#include "QtConfig.h"
#include "SslProxy.h"

namespace po = boost::program_options;

/////////////////////////////////////////////////////////////////////
void
parseCommandLine(int argc, char** argv,
	std::string& configFile)
{
  	const char *optarg;
   	bool err = false;

	po::options_description descripts("Options");
	descripts.add_options()
	("config,c", po::value<std::string>(&configFile)->implicit_value(""), "configuration file")
	("help,h",                                                            "help")
	;

	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, descripts), vm);
		po::notify(vm);
	}
	catch (...)
	{
		err++;
	}

	if (vm.count("config")) {
		if (configFile.size() == 0) {
			std::cout << "*** The config parameter requires a file path" << std::endl;
			err++;
		}
	}

	if (vm.count("help") || err) {
		std::cout << descripts << std::endl;
		std::cout << "Edit the configuration file (typically ~/.config/NCAR/Proxy.ini) to set configuration parameters" << std::endl;
		exit(1);
	}
}

/////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// Process command line options
	std::string configFile;
	parseCommandLine(argc, argv, configFile);

	// Get the configuration
	QtConfig* config;
	if (configFile.size()) {
		config = new QtConfig(configFile);
	} else {
		config = new QtConfig("NCAR", "Proxy");
	}

	// Configuration for one message
	SslProxy::InstMsgInfo msg;

	// Get configuration values. Default values will be created in the
	// configuration file if they don't exist, so that running the program
	// for the first time will create a configuration template.
	int proxyUdpPort           = config->getInt   ("ProxyUdpPort",    0);
	std::string proxyKeyFile    (config->getString("ProxyKeyFile",    "./proxy.key"));
	std::string proxyCertFile   (config->getString("ProxyCertFile",   "./proxy.crt"));
	int switchProxyPort        = config->getInt   ("SwitchProxyPort", 0);
	std::string switchHostName = config->getString("SwitchHostName",  "127.0.0.1");
	std::string switchCertFile = config->getString("SwitchCertFile",  "./switch.crt");
	std::string instName       = config->getString("InstrumentID",    "INSTRUMENT");

	// Configure a message
	/// @todo Rework the configuration to support multiple messages
	msg._msgId                 = config->getString("InstMsgID",       "INST");
	msg._destPort              = config->getInt   ("InstUdpPort",     0);
	msg._destIP                = config->getString("InstHostName",    "127.0.0.1");
    msg._broadcast             = config->getBool  ("InstMsgBroadcast", true);
    msg._instName              = instName;

    std::map<std::string, SslProxy::InstMsgInfo> messages;
    messages[msg._msgId] = msg;

    // If the port number is 0, it indicates that the user has not configured
	// the application yet. We wait until this point so that all of the default values
	// will have been added to the configuration file. Force them to take a stab at
	// configuration.
	if (switchProxyPort == 0 || msg._destPort == 0) {
		std::cout << "Please create a usable configuration by editing " << config->fileName() << std::endl;
		exit(1);
	}

	// add the server certificate to the CA database
	std::vector<std::string> caDatabase;
	caDatabase.push_back(proxyCertFile);
	caDatabase.push_back(switchCertFile);

	// Create the Qt application, which proides the event loop
	QCoreApplication app(argc, argv);

    // Create the proxy. It will try to connect with the switch.
	SslProxy proxy(
    		proxyUdpPort,
    		proxyKeyFile,
    		proxyCertFile,
    		switchHostName,
    		switchProxyPort,
    		caDatabase,
    		instName,
    		messages);

    return app.exec();
}
