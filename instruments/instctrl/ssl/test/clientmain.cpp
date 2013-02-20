#include <iostream>
#include <string>
#include "SslConnection.h"

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

    std::vector<QSslCertificate> extraCerts;
    // add the client certificate to the CA database
    extraCerts.push_back(QSslCertificate::fromPath(clientCertPath.c_str())[0]);

    // add additional certs to the database
    for (int i = 4; i < argc; i++) {
    	extraCerts.push_back(QSslCertificate::fromPath(argv[i])[0]);
    }

    Ssl::SslConnection client(
    		clientKeyPath,
    		QSslCertificate::fromPath(clientCertPath.c_str())[0],
    		"localhost",
    		50000,
    		extraCerts,
    		clientID);

    return app.exec();
}
