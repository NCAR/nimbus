#include <iostream>
#include <string>
#include <sstream>
#include "QtConfig.h"
#include "Proxy.h"

int main(int argc, char** argv)
{
	// Get the configuration
	QtConfig config("NCAR", "Proxy");

	if (argc != 1) {
		std::cout << "usage: " << argv[0] << std::endl;
		std::cout << "Edit " << config.fileName()
				<< " to set configuration parameters" << std::endl;
		exit(1);
	}

	// Get configuration values. Default values will be created in the
	// configuration file if they don't exist, so that running the program
	// for the first time will create a configuration template.
	std::string instrumentID   = config.getString("InstrumentID",    "TEST");
	int instUdpPort            = config.getInt   ("ProxyUdpPort",    0);
	std::string proxyKeyFile    (config.getString("ProxyKeyFile",    "./proxy.key"));
	std::string proxyCertFile   (config.getString("ProxyCertFile",   "./proxy.crt"));
	int switchProxyPort        = config.getInt   ("SwitchProxyPort", 0);
	std::string switchHostName = config.getString("SwitchHostName",  "127.0.0.1");
	std::string switchCertFile = config.getString("SwitchCertFile",  "./switch.crt");

	// If the port number is 0, it indicates that the user has not configured
	// the application yet. We wait until this point so that all of the default values
	// will have been added to the configuration file. Force them to take a stab at
	// configuration.
	if (switchProxyPort == 0) {
		std::cout << "Please create a usable configuration by editing " << config.fileName() << std::endl;
		exit(1);
	}

	// add the server certificate to the CA database
	std::vector<std::string> caDatabase;
	caDatabase.push_back(proxyCertFile);
	caDatabase.push_back(switchCertFile);

	// Create the Qt application, which proides the event loop
	QCoreApplication app(argc, argv);

    // Create the proxy. It will try to connect with the switch.
	Proxy proxy(
    		instUdpPort,
    		proxyKeyFile,
    		proxyCertFile,
    		switchHostName,
    		switchProxyPort,
    		caDatabase,
    		instrumentID);

    return app.exec();
}
