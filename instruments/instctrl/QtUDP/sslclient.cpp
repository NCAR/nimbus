#include "sslclient.h"

SslClient::SslClient(QWidget *parent)
	: QDialog(parent)
{
	status_ = new QLabel(tr("Enter port and server name to connect:"));
	connection_ = new QLabel;
	connection_->hide();
	portLabel_ = new QLabel(tr("Port:"));
	port_ = new QComboBox;
	port_->setEditable(true);
	uniquePorts_ = new QSet<QString>;
	hostLabel_ = new QLabel(tr("Host Name:"));
	hostName_ = new QLineEdit;
	clientLabel_ = new QLabel(tr("Client Name"));
	clientName_ = new QLineEdit;
	message_ = new QTextEdit;
	message_->hide();

	connectButton_ = new QPushButton(tr("Connect"));
	switchButton_ = new QPushButton(tr("Switch"));
	switchButton_->hide();
	sendButton_ = new QPushButton(tr("Send"));
	sendButton_->hide();
	writeButton_ = new QPushButton(tr("Write new message"));
	writeButton_->hide();
	quitButton_ = new QPushButton(tr("Quit"));
	quitButton_->hide();

	sslSocket_ = new QSslSocket(this);

	connect(connectButton_, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(sslSocket_, SIGNAL(connected()), this, SLOT(clientConnected()));
	connect(sslSocket_, SIGNAL(encrypted()), this, SLOT(clientEncrypted()));
	connect(sslSocket_, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	connect(sslSocket_, SIGNAL(encrypted()), this, SLOT(sendMode()));
	connect(writeButton_, SIGNAL(clicked()), this, SLOT(sendMode()));
	connect(sendButton_, SIGNAL(clicked()), this, SLOT(sendMessage()));
	connect(sslSocket_, SIGNAL(readyRead()), this, SLOT(readMode()));
	connect(switchButton_, SIGNAL(clicked()), this, SLOT(switchHostMode()));
	connect(quitButton_, SIGNAL(clicked()), this, SLOT(quitSession()));
	connect(sslSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this,
			SLOT(displayError(QAbstractSocket::SocketError)));
	connect(sslSocket_, SIGNAL(sslErrors(const QList<QSslError> &)), this,
			SLOT(displayError(const QList<QSslError> &)));
	connect(sslSocket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
			this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(switchButton_);
	buttonLayout->addWidget(connectButton_);
	buttonLayout->addWidget(sendButton_);
	buttonLayout->addWidget(writeButton_);
	buttonLayout->addWidget(quitButton_);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(connection_, 0, 0, 1, 2);
	mainLayout->addWidget(status_, 1, 0, 1, 2);
	mainLayout->addWidget(portLabel_, 2, 0);
	mainLayout->addWidget(port_, 2, 1);
	mainLayout->addWidget(hostLabel_, 3, 0);
	mainLayout->addWidget(hostName_, 3, 1);
	mainLayout->addWidget(clientLabel_, 4, 0);
	mainLayout->addWidget(clientName_, 4, 1);
	mainLayout->addWidget(message_, 5, 0, 1, 2);
	mainLayout->addLayout(buttonLayout, 6, 0, 1, 2);
	setLayout(mainLayout);
	setWindowTitle(tr("SSL Client"));
}

void SslClient::connectToServer()
{
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

		sslSocket_->abort();

		// PUT IN KEY AND CERTIFICATE STUFF HERE
//		QFile keyFile("certs/client.key");
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

		QFile keyFile("sslcert/cakey.key");
		QFile newClientFile("sslcert/cacert.crt");

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

		newClientFile.open(QIODevice::ReadOnly);

		QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, QByteArray("start"));
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

		QSslCertificate newClientCert(&newClientFile);

		QList<QSslCertificate> clientCertificates;
		clientCertificates.append(clientCert);
		clientCertificates.append(tikalClientCert);
		clientCertificates.append(shirazClientCert);
		clientCertificates.append(dropletClientCert);
		clientCertificates.append(sloopClientCert);
		clientCertificates.append(multiHostClientCert);
		clientCertificates.append(newClientCert);

		QList<QSslCertificate> serverCertificates;
		serverCertificates.append(serverCert);
		serverCertificates.append(tikalServerCert);
		serverCertificates.append(shirazServerCert);
		serverCertificates.append(dropletServerCert);
		serverCertificates.append(sloopServerCert);
		serverCertificates.append(multiHostServerCert);

		sslSocket_->setPrivateKey(key);
		sslSocket_->addCaCertificates(clientCertificates);
		sslSocket_->addCaCertificates(serverCertificates);

		sslSocket_->setLocalCertificate(newClientCert);

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
		QList<QSslError> expectedSslErrors;
		expectedSslErrors.append(error);
		expectedSslErrors.append(newError);
		expectedSslErrors.append(tikalError);
		expectedSslErrors.append(shirazError);
		expectedSslErrors.append(dropletError);
		expectedSslErrors.append(sloopError);

		sslSocket_->connectToHostEncrypted(hostName_->text(), portNumber);
		sslSocket_->ignoreSslErrors();
		QByteArray block;
		block.append(clientName_->text());
		sslSocket_->write(block);
	}
}

void SslClient::clientConnected()
{
	QString host = sslSocket_->localAddress().toString();
	connection_->setText(tr("Connected to %1 at port %2.")
							.arg(host).arg(sslSocket_->localPort()));
	connection_->show();
	quitButton_->show();
}

void SslClient::clientEncrypted()
{
	QString host = sslSocket_->localAddress().toString();
	if (sslSocket_->peerCertificate().isNull()) {
		connection_->setText(tr("Connected and encrypted, but certificate is null."));
	} else {
		QString subjectInfo;
		QString issuerInfo;

		//Fetchin the subject info
		subjectInfo.append(sslSocket_->peerCertificate().issuerInfo(QSslCertificate::Organization));
		subjectInfo.append(sslSocket_->peerCertificate().issuerInfo(QSslCertificate::CommonName));
		//Fetching effective and expiry dates
		subjectInfo.append(sslSocket_->peerCertificate().effectiveDate().toString());
		subjectInfo.append(sslSocket_->peerCertificate().expiryDate().toString());
		//Fetching the serial number
		subjectInfo.append(sslSocket_->peerCertificate().serialNumber());

		//Fetchin the issuer info
		issuerInfo.append(sslSocket_->peerCertificate().issuerInfo(QSslCertificate::Organization));
		issuerInfo.append(sslSocket_->peerCertificate().issuerInfo(QSslCertificate::CommonName));
		//Fetching effective and expiry dates
		issuerInfo.append(sslSocket_->peerCertificate().effectiveDate().toString());
		issuerInfo.append(sslSocket_->peerCertificate().expiryDate().toString());
		//Fetching the serial number
		issuerInfo.append(sslSocket_->peerCertificate().serialNumber());

		connection_->setText(tr("Connected and encrypted to %1 at port %2.\nSubject Info: %3\nIssuer Info: %4")
								.arg(host).arg(sslSocket_->localPort())
								.arg(subjectInfo).arg(issuerInfo));
	}
	connection_->show();
	quitButton_->show();
}

void SslClient::clientDisconnected()
{
	status_->hide();
	message_->hide();
	QString host = sslSocket_->localAddress().toString();
	connection_->setText(tr("Disconnected from %1.").arg(host));
	connection_->show();
}

void SslClient::sendMode()
{
	portLabel_->hide();
	port_->hide();
	hostLabel_->hide();
	hostName_->hide();
	clientLabel_->hide();
	clientName_->hide();
	connectButton_->hide();
	writeButton_->hide();

	status_->setText(tr("Write message below:"));

	status_->show();
	message_->show();
	sendButton_->show();
	switchButton_->show();
}

void SslClient::sendMessage()
{
	message_->hide();
	sendButton_->hide();
	connection_->hide();

	QString host = sslSocket_->localAddress().toString();

	QByteArray block;
	block.append(message_->toPlainText());
	sslSocket_->write(block);

	status_->setText(tr("Message sent to %1 on port %2.")
						.arg(host).arg(sslSocket_->localPort()));
	message_->clear();
	writeButton_->show();
}

void SslClient::readMode()
{
	qint64 available = sslSocket_->bytesAvailable();
	QByteArray newMessage = sslSocket_->read(available);
	status_->setWordWrap(true);
	status_->setText(tr("New message from server received:\n").append(newMessage));
	status_->show();
}

void SslClient::switchHostMode()
{
	connection_->hide();
	switchButton_->hide();
	sendButton_->hide();
	writeButton_->hide();
	message_->hide();

	sslSocket_->disconnectFromHost();

	status_->setText(tr("Enter new port and host:"));
	portLabel_->show();
	port_->show();
	hostLabel_->show();
	hostName_->show();
	clientLabel_->show();
	clientName_->show();
	connectButton_->show();
}

void SslClient::quitSession()
{
	sslSocket_->abort();
	close();
}

void SslClient::displayError(QAbstractSocket::SocketError socketError)
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

void SslClient::displayError(const QList<QSslError> & errors)
{
	QString errorList;
	for (int i = 0; i < errors.size(); ++i) {
		errorList.append(tr("Error %1: %2.\n").arg(i+1).arg(errors[i].errorString()));
	}
	QMessageBox::information(this, tr("SSL Error"), errorList);
}

void SslClient::slot_stateChanged (QAbstractSocket::SocketState state)
{
   qDebug() << "SslClient::slot_stateChanged(" << state << ")";
}

