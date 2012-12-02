#include <QtCore>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>

#include "QtConfig.h"
#include "Switch.h"

namespace po = boost::program_options;

void
parseCommandLine(int argc, char** argv,
	std::string& configFile)
{
  	const char *optarg;
   	bool err = false;

	po::options_description descripts("Options");
	descripts.add_options()
	("config,c", po::value<std::string>(&configFile)->implicit_value(""), "configuration file")
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

int main(int  argc, char** argv){

	std::string configFile;

	parseCommandLine(argc, argv, configFile);

	// Get the configuration
	QtConfig* config;
	if (configFile.size()) {
		config = new QtConfig(configFile);
	} else {
		config = new QtConfig("NCAR", "Switch");
	}

	int switchProxyPort = config->getInt("SwitchProxyPort", 0);

	std::string serverKey(config->getString("SwitchKeyFile", "./switch.key"));
	std::string serverCert(config->getString("SwitchCertFile", "./switch.crt"));

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
	if (switchProxyPort == 0) {
		std::cout << "Please create a usable configuration by editing " << config->fileName() << std::endl;
		exit(1);
	}

	// Create the Qt application (i.e. event loop)
	QCoreApplication app(argc, argv);

	// Create the switch. The switch creates:
	//  - an SslServer
	//  - a SwitchConnection to the remote switch
	//  - SslServer creates connections to Proxies.
	Switch swtch(serverKey, serverCert, switchProxyPort, caDatabase);

	// Run the event loop
	return app.exec();

}
