#include <iostream>
#include <string>
#include "SslClientConnection.h"

/// Test SslClientConection to an SSL server. The server port is hardwired to 5000.
/// Use with servermain.cpp. The server certificate must be provided as an extra cert in order
/// to avoid self-signed certificate errors.
int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cout << "usage: " << argv[0] << " clientID client_private_key_file client_cert_file [extra_cert_file ... extra_cert_file]" << std::endl;
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

    Ssl::SslClientConnection client(
    		clientKeyPath,
    		clientCertPath,
    		"localhost",
    		50000,
    		caDatabase,
    		clientID);

    return app.exec();
}
