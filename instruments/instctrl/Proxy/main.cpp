#include <iostream>
#include <string>
#include "Proxy.h"

int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cout << "usage: " << argv[0] << " clientID client_key_file client_cert_file [extra_cert_file ... extra_cert_file]" << std::endl;
        exit(1);
    }

    QCoreApplication app(argc, argv);

    std::string clientID(argv[1]);
    std::string clientKeyPath(argv[2]);
    std::string clientCertPath(argv[3]);

    std::vector<std::string> caDatabase;
    // add the client certificate to the CA database
    caDatabase.push_back(clientCertPath);

    // add additional certs to the database
    for (int i = 4; i < argc; i++) {
        caDatabase.push_back(argv[i]);
    }

    Proxy proxy(
    		12346,
    		clientKeyPath,
    		clientCertPath,
    		"localhost",
    		12345,
    		caDatabase,
    		clientID);

    return app.exec();
}
