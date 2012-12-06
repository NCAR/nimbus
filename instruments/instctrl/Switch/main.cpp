#include <QtCore>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>
#include "QtConfig.h"
#include "Switch.h"

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
		std::cout << "Edit the configuration file (typically ~/.config/NCAR/Switch.ini) to set configuration parameters" << std::endl;
		exit(1);
	}
}

/////////////////////////////////////////////////////////////////////
int main(int  argc, char** argv){

	// Process command line options
	std::string configFile;
	parseCommandLine(argc, argv, configFile);

	// Get the configuration
	QtConfig* config;
	if (configFile.size()) {
		config = new QtConfig(configFile);
	} else {
		config = new QtConfig("NCAR", "Switch");
	}

	// The port for proxy communications
	int switchProxyPort = config->getInt("SwitchProxyPort", 0);

	// Listen on this port for messages from the remote switch
	int switchLocalPort = config->getInt("SwitchLocalPort", 0);

	// The destination port for messages to the remote switch
	int switchRemotePort = config->getInt("SwitchRemotePort", 0);

	// The remote switch IP
	std::string switchRemoteIP(config->getString("SwitchRemoteIP", "127.0.0.1"));

	// The file containing the private key for the switch to proxy SSL link.
	// This file must be kept private!
	std::string serverKey(config->getString("SwitchKeyFile", "./switch.key"));

	// The file containing the private certificate for the switch to proxy SSL link.
	std::string serverCert(config->getString("SwitchCertFile", "./switch.crt"));

	// The file containing the key for symmetric cipher encryption over SwitchConnection.
	// This file must be kept private!
	std::string switchCipherKey(config->getString("SwitchCipherKey", "./udpcipher.key"));

	std::vector<std::string> caDatabase;
	caDatabase.push_back("./proxy.crt");
	caDatabase = config->getArray("ClientCerts", "ClientCertFile", caDatabase);
	// add the server certificate to the CA database
	caDatabase.push_back(serverCert);

	// add additional certs to the database
	for (int i = 3; i < argc; i++) {
		caDatabase.push_back(argv[i]);
	}

	// If the port number is 0, it indicates that the user has not configured
	// the application yet. We wait until this point so that all of the default values
	// will have been added to the configuration file. Force them to take a stab at
	// configuration.
	if (switchProxyPort == 0 || switchLocalPort == 0 || switchRemotePort == 0) {
		std::cout << "Please create a usable configuration by editing " << config->fileName()
				<< " (make sure ports and IP addresses are valid)"<< std::endl;
		exit(1);
	}

	// Create the Qt application (i.e. event loop)
	QCoreApplication app(argc, argv);

	// Create the switch. The switch creates:
	//  - an SslServer
	//  - a SwitchConnection to the remote switch
	//  - SslServer creates connections to Proxies.
	Switch swtch(
			serverKey,
			serverCert,
			switchProxyPort,
			caDatabase,
			switchLocalPort,
			switchRemoteIP,
			switchRemotePort,
			switchCipherKey);

	// Run the event loop
	return app.exec();

}
