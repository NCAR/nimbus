#include <QtGui>
#include <iostream>
#include <string>
#include "sslsocket.h"

int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cout << "usage: " << argv[0] << " client_key_file client_cert_file clientID [extra_cert_file ... extra_cert_file]" << std::endl;
        exit(1);
    }

    QApplication app(argc, argv);

    std::string clientKey(argv[1]);
    std::string clientCert(argv[2]);
    std::string clientID(argv[3]);

    std::vector<std::string> caDatabase;
    // add the client certificate to the CA database
    caDatabase.push_back(clientCert);

    // add additional certs to the database
    for (int i = 4; i < argc; i++) {
        caDatabase.push_back(argv[i]);
    }

    SSL::SslSocket client(clientKey, clientCert, "localhost", 50000, caDatabase, clientID);

    return app.exec();
}
