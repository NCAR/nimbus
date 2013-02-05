#include "SslServer.h"

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SslServer::SslServer(std::string keyFile,
		QSslCertificate sslCert,
		int port,
		std::vector<QSslCertificate> extraCerts,
		QObject * parent):
	QTcpServer(parent),
	_keyFile(keyFile),
	_sslCert(sslCert),
	_port(port),
	_extraCerts(extraCerts)
{

	// Add our certificate to the CA database
	_extraCerts.push_back(sslCert);

	// Listen on all network interfaces
	bool validSession = listen(QHostAddress::Any, port);

	if (!validSession) {
		qDebug() << (tr("Unable to start the server on port %1: %2.")
				.arg(port).arg(errorString()));
		exit(1);
	} else {
	}
}

/////////////////////////////////////////////////////////////////////
SslServer::~SslServer() {
}

/////////////////////////////////////////////////////////////////////
void SslServer::incomingConnection(int descriptor) {
	qDebug() << "Incoming connection on port" << _port << "(descriptor" << descriptor << ")";

	SslSocket* socket = new SslSocket(_keyFile, _sslCert, descriptor, _extraCerts);

	if (socket->socketDescriptor() != -1) {
		_sslSockets[socket] = socket;
		emit newConnection(socket);
	}

}

/////////////////////////////////////////////////////////////////////
void SslServer::showServerSockets() {
	for (std::map<SslSocket*, SslSocket*>::iterator i = _sslSockets.begin(); i != _sslSockets.end(); i++) {
		qDebug() << "** ServerSocket" << i->second->socketDescriptor() << ", State:" << i->second->state();
	}
}
