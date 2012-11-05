#include "sslserver.h"

/////////////////////////////////////////////////////////////////////
SslServer::SslServer(std::string keyFile,
		std::string certFile,
		int port,
		std::vector<std::string> caDatabase,
		QObject * parent):
	QTcpServer(parent),
	_keyFile(keyFile),
	_certFile(certFile),
	_port(port),
	_caDatabase(caDatabase)
{

	// Listen on all network interfaces
	bool validSession = listen(QHostAddress::Any, port);

	if (!validSession) {
		qDebug() << (tr("Unable to start the server: %1.")
							.arg(errorString()));
		exit(1);
	}
}

/////////////////////////////////////////////////////////////////////
SslServer::~SslServer() {

}

/////////////////////////////////////////////////////////////////////
void SslServer::incomingConnection(int descriptor) {
	qDebug() << "incoming Connection on port " << _port << " (descriptor " << descriptor << ")";
	SslSocket* socket = new SslSocket(_keyFile, _certFile, descriptor, _caDatabase);
}
