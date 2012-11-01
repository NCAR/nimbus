#include "sslsocket.h"
/////////////////////////////////////////////////////////////////////
SslSocket::SslSocket(std::string keyFile,
		std::string certFile,
		int descriptor,
		QObject * parent):
	QSslSocket(parent),
	_keyFile(keyFile),
	_certFile(certFile),
	_port(-1),
	_descriptor(descriptor),
	_serverHost("")
{
	qDebug() << "Create a server SslSocket" << descriptor;
	// Initialize the key and certificate and connect signals
	init();

	if (setSocketDescriptor(_descriptor)) {
		startServerEncryption();
	} else {
		qDebug() << "Unable to set socket descriptor for new connection";
	}
}

/////////////////////////////////////////////////////////////////////
SslSocket::SslSocket(std::string keyFile,
		std::string certFile,
		std::string serverHost,
		int port,
		QObject * parent):
	QSslSocket(parent),
	_keyFile(keyFile),
	_certFile(certFile),
	_port(port),
	_descriptor(-1),
	_serverHost(serverHost)
{
	qDebug() << "Create a client SslSocket to server" << serverHost.c_str() << "on port" << port;
	// Initialize the key and certificate and connect signals
	init();

	connectToHostEncrypted(_serverHost.c_str(), _port);
}

/////////////////////////////////////////////////////////////////////
SslSocket::~SslSocket() {

}

/////////////////////////////////////////////////////////////////////
void SslSocket::init() {

	//dumpCA();

	connect(this, SIGNAL(connected()), this, SLOT(connected()));
	connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(error(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(encrypted()), this, SLOT(encrypted()));
	connect(this, SIGNAL(sslErrors(const QList<QSslError>&)),
			this, SLOT(sslErrors(const QList<QSslError>&)));
	connect(this, SIGNAL(modeChanged(QSslSocket::SslMode)),
			this, SLOT(modeChanged(QSslSocket::SslMode)));

	// Require the peer to provide a certificate.
	setPeerVerifyMode(QSslSocket::VerifyPeer);

	if (_keyFile.size() > 0) {
		setPrivateKey(_keyFile.c_str());
		if (privateKey().isNull())
			qDebug() << "Null key with" << _keyFile.c_str();
	}
	if (_certFile.size() > 0) {
		setLocalCertificate(_certFile.c_str());
		if (localCertificate().isNull())
			qDebug() << "Null certificate with" << _certFile.c_str();
		else if (!localCertificate().isValid())
			qDebug() << "Invalid certificate with" << _certFile.c_str();
	}
}

/////////////////////////////////////////////////////////////////////
void SslSocket::dumpCA() {
	qDebug() << "***************** CA database *********************";
	QList<QSslCertificate> certs =  caCertificates();
	for (int i = 0; i < certs.size(); i++) {
		qDebug() << certs[i].serialNumber();
		qDebug() << "  " << "O/" << certs[i].subjectInfo(QSslCertificate::Organization)
				<< "OU/" << certs[i].subjectInfo(QSslCertificate::OrganizationalUnitName)
				<< "CN/" << certs[i].subjectInfo(QSslCertificate::CommonName);
		qDebug() << "  " << "O/" << certs[i].issuerInfo(QSslCertificate::Organization)
				<< "OU/" << certs[i].issuerInfo(QSslCertificate::OrganizationalUnitName)
				<< "CN/" << certs[i].issuerInfo(QSslCertificate::CommonName);
	}
	qDebug() << "***************************************************";
}

/////////////////////////////////////////////////////////////////////
void SslSocket::connected() {
	qDebug() << "Connected";
}

/////////////////////////////////////////////////////////////////////
void SslSocket::disconnected() {
	qDebug() << "Disconnected" << _descriptor;
}

/////////////////////////////////////////////////////////////////////
void SslSocket::error(QAbstractSocket::SocketError error) {
	qDebug() << "error " << error << ", disconnecting";
	disconnectFromHost();
}

/////////////////////////////////////////////////////////////////////
void SslSocket::encrypted() {
	QSslCertificate peerCert = peerCertificate();
	QString  O(peerCert.issuerInfo(QSslCertificate::Organization));
	QString OU(peerCert.issuerInfo(QSslCertificate::OrganizationalUnitName));
	qDebug() << "Encrypted, peer certificate: Organization:" << O << " Unit:" << OU;
}

/////////////////////////////////////////////////////////////////////
void SslSocket::modeChanged(QSslSocket::SslMode mode) {
	qDebug() << "Mode changed to" << mode;
}

/////////////////////////////////////////////////////////////////////
void SslSocket::sslErrors(const QList<QSslError>& errors) {
	// we only allow the selfSignedCertificate error. We can get multiple
	// ones if both client and server certificates are self-signed.
	for (int i = 0; i < errors.size(); i++) {
		if (errors[i].error() != QSslError::SelfSignedCertificate) {
			break;
		}

		// allow this set of SelfSignedCertificate errors to be ignored
		ignoreSslErrors();

		qDebug() << errors << ", ignored";
		return;
	}

	qDebug() << "SslErrors:" << errors;
}
