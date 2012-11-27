#include <iostream>
#include <string>
#include <sstream>
#include "Proxy.h"

int main(int argc, char** argv)
{
    if (argc < 6) {
        std::cout << "usage: " << argv[0] << " udp_port switch_port clientID client_key_file client_cert_file [extra_cert_file ... extra_cert_file]" << std::endl;
        exit(1);
    }

	std::stringstream s;
	s << argv[1];
	int udpport;
	s >> udpport;
	s.clear();
	s << argv[2];
	int switchport;
	s >> switchport;

    std::string clientID(argv[3]);
    std::string clientKeyPath(argv[4]);
    std::string clientCertPath(argv[5]);

    std::vector<std::string> caDatabase;
    // add the client certificate to the CA database
    caDatabase.push_back(clientCertPath);

    // add additional certs to the database
    for (int i = 5; i < argc; i++) {
        caDatabase.push_back(argv[i]);
    }

    QCoreApplication app(argc, argv);

    Proxy proxy(
    		udpport,
    		clientKeyPath,
    		clientCertPath,
    		"localhost",
    		switchport,
    		caDatabase,
    		clientID);

    return app.exec();
}
