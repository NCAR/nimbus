#include <QtGui>
#include <iostream>
#include <string>
#include "sslserver.h"

int main(int argc, char** argv)
{
	if (argc < 3) {
		std::cout << "usage: " << argv[0] << " server_key_file server_cert_file [extra_cert_file ... extra_cert_file]" << std::endl;
		exit(1);
	}

	QApplication app(argc, argv);

	std::string serverKey(argv[1]);
	std::string serverCert(argv[2]);

	std::vector<std::string> caDatabase;
	// add the server certificate to the CA database
	caDatabase.push_back(serverCert);

	// add additional certs to the database
	for (int i = 3; i < argc; i++) {
		caDatabase.push_back(argv[i]);
	}

	SSL::SslServer server(serverKey, serverCert, 50000, caDatabase);

	return app.exec();
}
