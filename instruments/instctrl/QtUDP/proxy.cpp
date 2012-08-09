#include "proxy.h"
#include "logx/Logging.h"

LOGGING("proxy");

Proxy::Proxy()
{
	status_ = new QLabel(tr("Enter port and key to connect client proxy:"));
	connection_ = new QLabel;
	connection_->hide();
	portLabel_ = new QLabel(tr("Port:"));
	addressLabel_ = new QLabel(tr("Instrument IP:"));
	clientLabel_ = new QLabel(tr("Instrument Key:"));

	port_ = new QComboBox;
	port_->setEditable(true);
	uniquePorts_ = new QSet<QString>;
	address_ = new QLineEdit;
	clientName_ = new QLineEdit;

	connectButton_ = new QPushButton(tr("Connect"));
	switchButton_ = new QPushButton(tr("New Connection"));
	switchButton_->hide();
	quitButton_ = new QPushButton(tr("Quit"));
	quitButton_->hide();

	udpInput_ = new QUdpSocket();
	sslSocket_ = new QSslSocket();

	connect(connectButton_, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(switchButton_, SIGNAL(clicked()), this, SLOT(switchHostMode()));
	connect(quitButton_, SIGNAL(clicked()), this, SLOT(quitSession()));

	connect(udpInput_, SIGNAL(readyRead()), this, SLOT(sendToServer()));
	connect(sslSocket_, SIGNAL(connected()), this, SLOT(clientConnected()));
	connect(sslSocket_, SIGNAL(encrypted()), this, SLOT(clientEncrypted()));
	connect(sslSocket_, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	connect(sslSocket_, SIGNAL(encrypted()), this, SLOT(sendMode()));
	connect(sslSocket_, SIGNAL(readyRead()), this, SLOT(readMode()));
	connect(sslSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
	connect(sslSocket_, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(displayError(const QList<QSslError> &)));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(switchButton_);
	buttonLayout->addWidget(connectButton_);
	buttonLayout->addWidget(quitButton_);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(connection_, 0, 0, 1, 2);
	mainLayout->addWidget(status_, 1, 0, 1, 2);
	mainLayout->addWidget(portLabel_, 2, 0);
	mainLayout->addWidget(port_, 2, 1);
	mainLayout->addWidget(addressLabel_, 3, 0);
	mainLayout->addWidget(address_, 3, 1);
	mainLayout->addWidget(clientLabel_, 4, 0);
	mainLayout->addWidget(clientName_, 4, 1);
	mainLayout->addLayout(buttonLayout, 5, 0, 1, 2);
	setLayout(mainLayout);
	setWindowTitle(tr("Proxy"));
}

void Proxy::connectToServer()
{
	QHostAddress ip;
	bool validAddress = ip.setAddress(address_->text());
	QString portString = port_->currentText();
	bool validPort;
	int portNumber = portString.toInt(&validPort);

	if (!validAddress) {
		status_->setText(tr("Invalid IP address. Retry:"));
		return;
	} else if (!validPort || portNumber > 65535 || portNumber < 0) {
		status_->setText(tr("Invalid port number. Retry:"));
		return;
	} else {
		sslSocket_->abort();

		// PUT IN KEY AND CERTIFICATE STUFF HERE
		QFile keyFile("certs/client.key");
		QFile clientFile("certs/client.crt");
		QFile serverFile("certs/server.crt");
		QFile tikalClientFile("certs/tikal_client.crt");
		QFile tikalServerFile("certs/tikal_server.crt");
		QFile shirazClientFile("certs/shiraz_client.crt");
		QFile shirazServerFile("certs/shiraz_server.crt");
		QFile dropletClientFile("certs/droplet_client.crt");
		QFile dropletServerFile("certs/droplet_server.crt");
		QFile sloopClientFile("certs/sloop_client.crt");
		QFile sloopServerFile("certs/sloop_server.crt");
		QFile multiHostClientFile("certs/san_client.crt");
		QFile multiHostServerFile("certs/san_server.crt");

		QFile eolServerFile("certs/eol-rt-data_server.crt");

		keyFile.open(QIODevice::ReadOnly);
		clientFile.open(QIODevice::ReadOnly);
		serverFile.open(QIODevice::ReadOnly);
		tikalClientFile.open(QIODevice::ReadOnly);
		tikalServerFile.open(QIODevice::ReadOnly);
		shirazClientFile.open(QIODevice::ReadOnly);
		shirazServerFile.open(QIODevice::ReadOnly);
		dropletClientFile.open(QIODevice::ReadOnly);
		dropletServerFile.open(QIODevice::ReadOnly);
		sloopClientFile.open(QIODevice::ReadOnly);
		sloopServerFile.open(QIODevice::ReadOnly);
		multiHostClientFile.open(QIODevice::ReadOnly);
		multiHostServerFile.open(QIODevice::ReadOnly);

		eolServerFile.open(QIODevice::ReadOnly);

		QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, QByteArray("client"));
		QSslCertificate clientCert(&clientFile);
		QSslCertificate serverCert(&serverFile);
		QSslCertificate tikalClientCert(&tikalClientFile);
		QSslCertificate tikalServerCert(&tikalServerFile);
		QSslCertificate shirazClientCert(&shirazClientFile);
		QSslCertificate shirazServerCert(&shirazServerFile);
		QSslCertificate dropletClientCert(&dropletClientFile);
		QSslCertificate dropletServerCert(&dropletServerFile);
		QSslCertificate sloopClientCert(&sloopClientFile);
		QSslCertificate sloopServerCert(&sloopServerFile);
		QSslCertificate multiHostClientCert(&multiHostClientFile);
		QSslCertificate multiHostServerCert(&multiHostServerFile);

		QSslCertificate eolServerCert(&eolServerFile);

		QList<QSslCertificate> clientCertificates;
		clientCertificates.append(clientCert);
		clientCertificates.append(tikalClientCert);
		clientCertificates.append(shirazClientCert);
		clientCertificates.append(dropletClientCert);
		clientCertificates.append(sloopClientCert);
		clientCertificates.append(multiHostClientCert);

		QList<QSslCertificate> serverCertificates;
		serverCertificates.append(serverCert);
		serverCertificates.append(tikalServerCert);
		serverCertificates.append(shirazServerCert);
		serverCertificates.append(dropletServerCert);
		serverCertificates.append(sloopServerCert);
		serverCertificates.append(multiHostServerCert);
		serverCertificates.append(eolServerCert);

		sslSocket_->setPrivateKey(key);
		sslSocket_->addCaCertificates(clientCertificates);
		sslSocket_->addCaCertificates(serverCertificates);

		sslSocket_->setLocalCertificate(multiHostClientCert);

		if (!sslSocket_->localCertificate().isValid()) {
			qDebug("Invalid client certificate");
			return;
		}

		QMultiMap<QSsl::AlternateNameEntryType, QString> alternates = sslSocket_->localCertificate().alternateSubjectNames();
		if (alternates.isEmpty()) {
			qDebug("No alternates in client certificate.");
		} else {
			qDebug() << "Subject Alternate Names for client:\n" << alternates;
		}

		QSslError error(QSslError::SelfSignedCertificate, serverCert);
		QSslError newError(QSslError::SelfSignedCertificate, multiHostServerCert);
		QSslError tikalError(QSslError::SelfSignedCertificate, tikalServerCert);
		QSslError shirazError(QSslError::SelfSignedCertificate, shirazServerCert);
		QSslError dropletError(QSslError::SelfSignedCertificate, dropletServerCert);
		QSslError sloopError(QSslError::SelfSignedCertificate, sloopServerCert);
		QSslError eolError(QSslError::SelfSignedCertificate, eolServerCert);

		QList<QSslError> expectedSslErrors;
		expectedSslErrors.append(error);
		expectedSslErrors.append(newError);
		expectedSslErrors.append(tikalError);
		expectedSslErrors.append(shirazError);
		expectedSslErrors.append(dropletError);
		expectedSslErrors.append(sloopError);
		expectedSslErrors.append(eolError);

		// Server connection to eol-rt-data is hardcoded here
		// Ignore only self-signed error on approved certificates during SSL connection
		sslSocket_->connectToHostEncrypted("eol-rt-data.guest.ucar.edu", 80);
		sslSocket_->ignoreSslErrors(expectedSslErrors);

		// Send instrument key to server to identify client
		QByteArray block;
		block.append(clientName_->text());
		block.append(",");
		block.append(portString);
		block.append(",");
		block.append(address_->text());
		sslSocket_->write(block);
	}
}

void Proxy::clientConnected()
{
	QString host = sslSocket_->peerAddress().toString();
	connection_->setText(tr("Connected to %1 at port %2.")
							.arg(host).arg(sslSocket_->peerPort()));
	connection_->show();
	quitButton_->show();
}

void Proxy::clientEncrypted()
{
	QString host = sslSocket_->peerAddress().toString();
	connection_->setText(tr("Connected and encrypted to %1 at port %2.")
							.arg(host).arg(sslSocket_->peerPort()));
	connection_->show();
	quitButton_->show();
}

void Proxy::clientDisconnected()
{
	QString host = sslSocket_->peerAddress().toString();
	connection_->setText(tr("Disconnected from %1.").arg(host));
	connection_->show();
}

void Proxy::readMode()
// Data sent over the SslSocket's connection should always be either the multiple instrument
// host error message or datagram status packets from the instrument
{
	qint64 available = sslSocket_->bytesAvailable();
	QByteArray newMessage = sslSocket_->read(available);
	QString clientErrorString(tr("This instrument is already in use. Reconnect with new key."));

	if (newMessage == clientErrorString) {
		status_->setText(clientErrorString);
		sslSocket_->disconnectFromHost();
	} else {
		status_->setWordWrap(true);
		status_->setText(tr("New message from server received:\n").append(newMessage));
		ILOG << newMessage.data();
	}
	status_->show();
}

void Proxy::sendMode()
{
	portLabel_->hide();
	port_->hide();
	addressLabel_->hide();
	address_->hide();
	clientLabel_->hide();
	clientName_->hide();
	connectButton_->hide();

	QString portString = port_->currentText();
	bool validPort;
	int portNumber = portString.toInt(&validPort);
	if (!validPort || portNumber > 65535 || portNumber < 0) {
		status_->setText(tr("Invalid port number. Retry:"));
		return;
	} else {
		// If entered port is new, must add it to the combobox and
		// set it as the current item
		if (!uniquePorts_->contains(portString)) {
			uniquePorts_->insert(portString);
			port_->addItem(portString);
			int newPortIndex = port_->findText(portString);
			port_->setCurrentIndex(newPortIndex);
		}
		udpInput_->bind(portNumber);
		status_->setText(tr("Now set to receive and transfer datagrams."));
	}

	status_->show();
	switchButton_->show();
}

void Proxy::sendToServer()
{
	while (udpInput_->hasPendingDatagrams()) {
		QByteArray newDatagram;
		newDatagram.resize(udpInput_->pendingDatagramSize());
		udpInput_->readDatagram(newDatagram.data(), newDatagram.size());

		sslSocket_->write(newDatagram.data(), newDatagram.size());
	}
}

void Proxy::switchHostMode()
{
	connection_->hide();
	switchButton_->hide();

	sslSocket_->disconnectFromHost();
	udpInput_->disconnectFromHost();

	status_->setText(tr("Enter new port and host:"));
	portLabel_->show();
	port_->show();
	addressLabel_->show();
	address_->show();
	clientLabel_->show();
	clientName_->show();
	connectButton_->show();
}

void Proxy::quitSession()
{
	sslSocket_->abort();
	udpInput_->abort();
	close();
}

// If any errors happen (SSL or otherwise), a pop up window will appear to
// inform user what error occurred.
void Proxy::displayError(QAbstractSocket::SocketError socketError)
{
	switch (socketError) {
	case QAbstractSocket::RemoteHostClosedError:
		break;
	case QAbstractSocket::HostNotFoundError:
		QMessageBox::information(this, tr("Error"),
								tr("The host was not found. Please check the host name "
									"and port settings."));
		break;
	case QAbstractSocket::ConnectionRefusedError:
		QMessageBox::information(this, tr("Error"),
								tr("The connection was refused by the peer. Make sure the "
									"server is running and check that the host name and "
									"port settings are correct."));
		break;
	default:
		QMessageBox::information(this, tr("Error"),
								tr("The following error occurred: %1.")
								.arg(sslSocket_->errorString()));
		break;
	}
}

void Proxy::displayError(const QList<QSslError> & errors)
{
	QString errorList;
	for (int i = 0; i < errors.size(); ++i) {
		errorList.append(tr("Error %1: %2.\n").arg(i+1).arg(errors[i].errorString()));
	}
	QMessageBox::information(this, tr("SSL Error"), errorList);
}

