#include "sslserver.h"

using namespace SSL;

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
		qDebug() << (tr("Unable to start the server: %1.").arg(errorString()));
		exit(1);
	} else {
		qDebug() << "Server is ready";
	}
}

/////////////////////////////////////////////////////////////////////
SslServer::~SslServer() {
}

/////////////////////////////////////////////////////////////////////
void SslServer::incomingConnection(int descriptor) {
	qDebug() << "Incoming connection on port" << _port << "(descriptor" << descriptor << ")";
	SslSocket* socket = new SslSocket(_keyFile, _certFile, descriptor, _caDatabase);
	if (socket->socketDescriptor() != -1)
		_sslSockets.push_back(socket);
}

/////////////////////////////////////////////////////////////////////
void SslServer::showServerSockets() {
	for (int i = 0; i < _sslSockets.size(); i++) {
		qDebug() << "Socket" << _sslSockets[i]->socketDescriptor() << ", State:" << _sslSockets[i]->state();
	}
}
