#include <QtCore>
#include <iostream>
#include <sstream>
#include <string>

#include "QtConfig.h"
#include "Switch.h"

int main(int  argc, char** argv){

	// Get the configuration
	QtConfig config("NCAR", "Switch");

	if (argc != 1) {
		std::cout << "usage: " << argv[0] << std::endl;
		std::cout << "Edit " << config.fileName()
				<< " to set configuration parameters" << std::endl;
		exit(1);
	}

	int port = config.getInt("SwitchProxyPort", 0);

	std::string serverKey(config.getString("SwitchKeyFile", ""));
	std::string serverCert(config.getString("SwitchCertFile", ""));

	std::vector<std::string> caDatabase;
	caDatabase = config.getArray("ClientCerts", "ClientCertFile", caDatabase);
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
	if (port == 0) {
		std::cout << "Please create a usable configuration by editing " << config.fileName() << std::endl;
		exit(1);
	}

	// Create the Qt application (i.e. event loop)
	QCoreApplication app(argc, argv);

	// Create the switch. The switch creates:
	//  - an SslServer
	//  - a SwitchConnection to the remote switch
	//  - SslServer creates connections to Proxies.
	Switch swtch(serverKey, serverCert, port, caDatabase);

	// Run the event loop
	return app.exec();

}
