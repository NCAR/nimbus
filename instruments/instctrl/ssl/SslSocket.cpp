#include "sslsocket.h"
#include <sstream>

using namespace SSL;

/////////////////////////////////////////////////////////////////////
SslSocket::SslSocket(
		std::string keyFile,
		std::string certFile,
		int descriptor,
		std::vector<std::string> caDatabase,
		QObject * parent):
	QSslSocket(parent),
	_keyFile(keyFile),
	_certFile(certFile),
	_port(-1),
	_descriptor(descriptor),
	_serverHost(""),
	_state(SS_Unconnected),
	_caDatabase(caDatabase)
{
	qDebug() << "Create a ServerSocket with descriptor" << descriptor;
	// Initialize the key and certificate and connect signals
	init();

	if (setSocketDescriptor(_descriptor)) {
		std::stringstream ss;
		ss << _descriptor;
		_socketID += ss.str();

		startServerEncryption();
	} else {
		qDebug() << "Unable to set socket descriptor" << descriptor << "for new connection";
	}
}

/////////////////////////////////////////////////////////////////////
SslSocket::SslSocket(std::string keyFile,
		std::string certFile,
		std::string serverHost,
		int port,
		std::vector<std::string> caDatabase,
		QObject * parent):
	QSslSocket(parent),
	_keyFile(keyFile),
	_certFile(certFile),
	_port(port),
	_descriptor(-1),
	_serverHost(serverHost),
	_state(SS_Unconnected),
	_caDatabase(caDatabase)
{
	qDebug() << "Create client socket to connect to server \""
			 << serverHost.c_str() << "\" on port" << port;
	// Initialize the key and certificate and connect signals
	init();

	connectToHostEncrypted(_serverHost.c_str(), _port);
}

/////////////////////////////////////////////////////////////////////
SslSocket::~SslSocket() {
}

/////////////////////////////////////////////////////////////////////
void SslSocket::init() {
	// Require the peer to provide a certificate.
	setPeerVerifyMode(QSslSocket::VerifyPeer);

	// set the CA database from the list of certificates.
	setCAdatabase();

	// connect the signals
	connectSignals();

	// Set and validate private key and certificate
	setPrivateKey(_keyFile.c_str());
	if (privateKey().isNull()) {
		qDebug() << "Invalid key specified in " << _keyFile.c_str();
	}

	setLocalCertificate(_certFile.c_str());
	if (localCertificate().isNull() || !localCertificate().isValid()) {
			qDebug() << "Invalid certificate specified in " << _certFile.c_str();
	}
}

/////////////////////////////////////////////////////////////////////
void SslSocket::connected() {
	qDebug() << _socketID.c_str() << "connected";

	_state = SS_Connected;
	emit stateChanged(_state);
}

/////////////////////////////////////////////////////////////////////
void SslSocket::disconnected() {
	qDebug() << _socketID.c_str() << "disconnected";

	_state = SS_Disconnected;
	emit stateChanged(_state);
}

/////////////////////////////////////////////////////////////////////
void SslSocket::socketError(QAbstractSocket::SocketError error) {
	qDebug() << _socketID.c_str() << "connection failed:" << errorString();
	emit stateChanged(SS_SocketError);
}

/////////////////////////////////////////////////////////////////////
void SslSocket::encrypted() {
	QSslCertificate peerCert = peerCertificate();
	QString  O(peerCert.issuerInfo(QSslCertificate::Organization));
	QString OU(peerCert.issuerInfo(QSslCertificate::OrganizationalUnitName));
	qDebug() << _socketID.c_str() << "encrypted, peer certificate: Organization:" << O << " Unit:" << OU;

	_state = SS_Encrypted;
	emit stateChanged(_state);
}

/////////////////////////////////////////////////////////////////////
void SslSocket::modeChanged(QSslSocket::SslMode mode) {
	qDebug() << _socketID.c_str() << "mode changed to" << mode;
}

/////////////////////////////////////////////////////////////////////
void SslSocket::sslErrors(const QList<QSslError>& errors) {
	qDebug() << _socketID.c_str() << "SslErrors:" << errors;
}

/////////////////////////////////////////////////////////////////////
void SslSocket::setCAdatabase() {
	// Collect the certificates
	QList<QSslCertificate> certs;
	for (int i = 0; i < _caDatabase.size(); i++) {
		QFile certFile(_caDatabase[i].c_str());
		certFile.open(QIODevice::ReadOnly);
		QSslCertificate cert(&certFile);
		if (cert.isNull() || !cert.isValid()) {
			qDebug() << _socketID.c_str() << "invalid certificate specified in" << _caDatabase[i].c_str();
		} else {
			certs.append(cert);
		}
	}

	// Add these certificates to the CAdatabase
	addCaCertificates(certs);

	// display the CA database
	//dumpCA();
}

/////////////////////////////////////////////////////////////////////
void SslSocket::connectSignals() {
	connect(this, SIGNAL(connected()),                      this, SLOT(connected()));
	connect(this, SIGNAL(encrypted()),                      this, SLOT(encrypted()));
	connect(this, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(modeChanged(QSslSocket::SslMode)));
	connect(this, SIGNAL(disconnected()),                   this, SLOT(disconnected()));

	connect(this, SIGNAL(sslErrors(const QList<QSslError>&)),  this, SLOT(sslErrors(const QList<QSslError>&)));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
}

/////////////////////////////////////////////////////////////////////
void SslSocket::dumpCA() {
	qDebug() << "***************** CA database *********************";
	QList<QSslCertificate> certs =  caCertificates();
	for (int i = 0; i < certs.size(); i++) {
		qDebug() << certs[i].serialNumber();
		qDebug() << "  Subject: " << "O/" << certs[i].subjectInfo(QSslCertificate::Organization)
				<< "OU/" << certs[i].subjectInfo(QSslCertificate::OrganizationalUnitName)
				<< "CN/" << certs[i].subjectInfo(QSslCertificate::CommonName);
		qDebug() << "  Issuer: " << "O/" << certs[i].issuerInfo(QSslCertificate::Organization)
				<< "OU/" << certs[i].issuerInfo(QSslCertificate::OrganizationalUnitName)
				<< "CN/" << certs[i].issuerInfo(QSslCertificate::CommonName);
	}
	qDebug() << "***************************************************";
}

/////////////////////////////////////////////////////////////////////
SslSocket::SocketState SslSocket::state() {
	return _state;
}

/////////////////////////////////////////////////////////////////////
std::string SslSocket::socketID() {
	return _socketID;
}
