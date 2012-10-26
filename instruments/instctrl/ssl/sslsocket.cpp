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
	// Initialize the key and certificate and connect signals
	init();

	if (setSocketDescriptor(_descriptor)) {
		setPeerVerifyMode(QSslSocket::VerifyPeer);
		startServerEncryption();
	} else {
		qDebug() << "unable to set socket descriptor for new connection";
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
	connect(this, SIGNAL(sslErrors(const QList<QSslError>&)),
			this, SLOT(sslErrors(const QList<QSslError>&)));
	connect(this, SIGNAL(modeChanged(QSslSocket::SslMode)),
			this, SLOT(modeChanged(QSslSocket::SslMode)));

	// Require the peer to provide a certificate.
	setPeerVerifyMode(QSslSocket::VerifyPeer);

	if (_keyFile.size() > 0) {
		setPrivateKey(_keyFile.c_str());
	}
	if (_certFile.size() > 0) {
		setLocalCertificate(_certFile.c_str());
	}
}

/////////////////////////////////////////////////////////////////////
void SslSocket::connected() {
	qDebug() << "connected";
}

/////////////////////////////////////////////////////////////////////
void SslSocket::disconnected() {
	qDebug() << "disconnected";
}

/////////////////////////////////////////////////////////////////////
void SslSocket::encrypted() {
	QSslCertificate peerCert = peerCertificate();
	QString  O(peerCert.issuerInfo(QSslCertificate::Organization));
	QString OU(peerCert.issuerInfo(QSslCertificate::OrganizationalUnitName));
	qDebug() << "encrypted, peer certificate: Organization:" << O << " Unit:" << OU;
}

/////////////////////////////////////////////////////////////////////
void SslSocket::modeChanged(QSslSocket::SslMode mode) {
	qDebug() << "mode changed to " << mode;
}

/////////////////////////////////////////////////////////////////////
void SslSocket::sslErrors(const QList<QSslError>& errors) {

	// we only allow the unsigned certificate error. We can get
	// multiple ones if both client and server certificates are
	// unsigned
	for (int i = 0; i < errors.size(); i++) {
		if (errors[i].error() != QSslError::SelfSignedCertificate) {
			break;
		}

		// allow this set of SelfSignedCertificate errors to be ignored
		ignoreSslErrors();

		qDebug() << errors << ", ignored";
		return;
	}

	qDebug() << "sslErrors: " << errors;
}
