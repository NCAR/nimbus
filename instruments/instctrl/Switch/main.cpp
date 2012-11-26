#include <QtCore>
#include <iostream>
#include <sstream>
#include <string>
#include "Switch.h"

int main(int  argc, char** argv)
{

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

	Switch swtch(serverKey, serverCert, port, caDatabase);

	return app.exec();

}
