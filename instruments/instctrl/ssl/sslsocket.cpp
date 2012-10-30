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
	connect(this, SIGNAL(connected()), this, SLOT(connected()));
	connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(this, SIGNAL(encrypted()), this, SLOT(encrypted()));
	connect(this, SIGNAL(peerVerifyError(const QSslError&)),
			this, SLOT(peerVerifyError(const QSslError&)));
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
void SslSocket::connected() {
	qDebug() << "Connected";
}

/////////////////////////////////////////////////////////////////////
void SslSocket::disconnected() {
	qDebug() << "Disconnected" << _descriptor;
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
void SslSocket::peerVerifyError(const QSslError& error) {
	qDebug() << "peerVerifyError:" << error;
	QList<QSslError> errors;
	errors.append(error);
	//sslErrors(errors);
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
