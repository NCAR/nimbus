#include <QtCore>
#include <iostream>
#include <sstream>
#include <string>
#include "Switch.h"

int main(int  argc, char** argv)
{
	QtConfig config("NCAR", "Switch");

	// mkae sure that there are defaults set for the configuration parameters.
	// Recall that the QtConfig::get functions provide a default value that
	// will be created if there isn't a value already in the configuration.
	config.getInt("SwitchPort", 12345);
	config.getString("SwitchKeyFile", "./switch.key");
	config.getString("SwitchCertFile", "./switch.cert");

	if (argc < 4) {
		std::cout << "usage: " << argv[0] << " port private_key_file certificate_file [extra_cert_file ... extra_certFile]" << std::endl;
		exit(1);
	}

	std::stringstream s;
	s << argv[1];
	int port;
	s >> port;

	std::string serverKey(argv[2]);
	std::string serverCert(argv[3]);

	std::vector<std::string> caDatabase;
	// add the server certificate to the CA database
	caDatabase.push_back(serverCert);

	// add additional certs to the database
	for (int i = 3; i < argc; i++) {
		caDatabase.push_back(argv[i]);
	}

	QCoreApplication app(argc, argv);

	Switch swtch(config, serverKey, serverCert, port, caDatabase);

	return app.exec();

}
