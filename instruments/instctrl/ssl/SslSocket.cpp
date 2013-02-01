#include "SslSocket.h"
#include <sstream>

using namespace Ssl;

/////////////////////////////////////////////////////////////////////
SslSocket::SslSocket(
		std::string keyFile,
		QSslCertificate sslCert,
		int descriptor,
		std::vector<QSslCertificate> extraCerts,
		QObject * parent):
	QSslSocket(parent),
	_keyFile(keyFile),
	_sslCert(sslCert),
	_port(-1),
	_descriptor(descriptor),
	_serverHost(""),
	_state(SS_Unconnected),
	_extraCerts(extraCerts)
{
	// Initialize the key and certificate and connect signals
	init();

	if (setSocketDescriptor(_descriptor)) {
		startServerEncryption();
	} else {
		qDebug() << "Unable to set socket descriptor" << descriptor << "for new connection";
	}
}

/////////////////////////////////////////////////////////////////////
SslSocket::SslSocket(
		std::string keyFile,
		QSslCertificate sslCert,
		std::string serverHost,
		int port,
		std::vector<QSslCertificate> extraCerts,
		QObject * parent):
	QSslSocket(parent),
	_keyFile(keyFile),
	_sslCert(sslCert),
	_port(port),
	_descriptor(-1),
	_serverHost(serverHost),
	_state(SS_Unconnected),
	_extraCerts(extraCerts)
{

	// Initialize the key and certificate and connect signals.
	init();

	// Initiate the SSSL connection. connectToHostEncrypted() is
	// a function if QSslSocket.
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

	setLocalCertificate(_sslCert);
	if (localCertificate().isNull() || !localCertificate().isValid()) {
			qDebug() << "Invalid certificate specified in " << _sslCert;
	}
}

/////////////////////////////////////////////////////////////////////
void SslSocket::connected() {
	_state = SS_Connected;
	emit stateChanged(_state);
}

/////////////////////////////////////////////////////////////////////
void SslSocket::disconnected() {
	_state = SS_Disconnected;
	emit stateChanged(_state);
}

/////////////////////////////////////////////////////////////////////
void SslSocket::socketError(QAbstractSocket::SocketError errorCode) {
	std::string errorMsg = errorString().toStdString();
	emit sslSocketError(errorCode, errorMsg);
}

/////////////////////////////////////////////////////////////////////
void SslSocket::sslErrors(const QList<QSslError>& errors) {
	qDebug() << "sslErrors:" << errors;
}

/////////////////////////////////////////////////////////////////////
void SslSocket::encrypted() {
	QSslCertificate peerCert = peerCertificate();
	//QString  O(peerCert.issuerInfo(QSslCertificate::Organization));
	//QString OU(peerCert.issuerInfo(QSslCertificate::OrganizationalUnitName));
	//qDebug() << "SslSocket encrypted, peer certificate: Organization:"
	//		<< O << " Unit:" << OU;

	_state = SS_Encrypted;
	emit stateChanged(_state);
}

/////////////////////////////////////////////////////////////////////
void SslSocket::modeChanged(QSslSocket::SslMode mode) {
	//if (mode == QSslSocket::SslClientMode)
	//	qDebug() << "SslSocket changed to Client mode";
	//if (mode == QSslSocket::SslServerMode)
	//	qDebug() << "SslSocket changed to Server mode";
}

/////////////////////////////////////////////////////////////////////
void SslSocket::setCAdatabase() {
	// Collect the certificates
	QList<QSslCertificate> certs;
	for (int i = 0; i < _extraCerts.size(); i++) {
		certs.append(_extraCerts[i]);
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
