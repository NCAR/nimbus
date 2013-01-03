#include <QtCore>
#include <iostream>
#include <string>
#include "Server.h"

/// A test application for testing SslServer. The server port number is hardwired to 50000.
/// Use with clientmain.cpp. The client certificate must be provided as an extra cert in order
/// to avoid self-signed certificate errors.
int main(int  argc, char** argv)
{

	if (argc < 3) {
		std::cout << "usage: " << argv[0] << " server_private_key_file server_cert_file [extra_cert_file ... extra_certFile]" << std::endl;
		exit(1);
	}

	QCoreApplication app(argc, argv);

	std::string serverKey(argv[1]);
	std::string serverCert(argv[2]);

	std::vector<std::string> caDatabase;
	// add the server certificate to the CA database
	caDatabase.push_back(serverCert);

	// add additional certs to the database
	for (int i = 3; i < argc; i++) {
		caDatabase.push_back(argv[i]);
	}

	Server server(serverKey, serverCert, 50000, caDatabase);

	return app.exec();

}
