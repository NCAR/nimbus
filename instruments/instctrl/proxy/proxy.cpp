#include "proxy.h"
#include "logx/Logging.h"

LOGGING("proxy");
static QString certDir("/home/local/raf/instruments/instctrl/QtUDP/certs");

Proxy::Proxy()
{
	//logx::LogToFile("ProxyLog");
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
		QFile keyFile(certDir + QString("/client.key"));
		//QFile clientFile(certDir + QString("/client.crt"));
		//QFile serverFile(certDir + QString("/server.crt"));
		//QFile clientFile(certDir + QString("/newclient.crt"));
		//QFile newServerFile(certDir + QString("/newserver.crt"));
		//QFile clientFile(certDir + QString("/tikal_client.crt"));
		//QFile tikalServerFile(certDir + QString("/tikal_server.crt"));
		//QFile clientFile(certDir + QString("/newshiraz.crt"));
		//QFile clientFile(certDir + QString("/shiraz.crt"));
		//QFile shirazServerFile(certDir + QString("/shiraz_server.crt"));
		QFile clientFile(certDir + QString("/droplet_client.crt"));
		//QFile dropletServerFile(certDir + QString("/droplet_server.crt"));
		//QFile sloopClientFile(certDir + QString("/sloop_client.crt"));
		//QFile sloopServerFile(certDir + QString("/sloop_server.crt"));
		//QFile multiHostClientFile(certDir + QString("/san_client.crt"));
		//QFile multiHostServerFile(certDir + QString("/san_server.crt"));

		QFile eolServerFile(certDir + QString("/eol-rt-data_server.crt"));
		QString notice;
		notice = "Key File: "+keyFile.fileName();
		qDebug(notice.toStdString().c_str());
		notice = "Cert File: "+clientFile.fileName();
		qDebug(notice.toStdString().c_str());

		if (!keyFile.open(QIODevice::ReadOnly)) {
		   qDebug("Could not open key file: ");
                   qDebug(keyFile.fileName().toStdString().c_str());
		   return;
                }
		if (!clientFile.open(QIODevice::ReadOnly)) {
		   qDebug("Could not open proxy certificate file: ");
                   qDebug(clientFile.fileName().toStdString().c_str());
		   return;
		}
		//serverFile.open(QIODevice::ReadOnly);
		//newClientFile.open(QIODevice::ReadOnly);
		//newServerFile.open(QIODevice::ReadOnly);
		//tikalClientFile.open(QIODevice::ReadOnly);
		//tikalServerFile.open(QIODevice::ReadOnly);
		//shirazClientFile.open(QIODevice::ReadOnly);
		//shirazServerFile.open(QIODevice::ReadOnly);
		//dropletClientFile.open(QIODevice::ReadOnly);
		//dropletServerFile.open(QIODevice::ReadOnly);
		//sloopClientFile.open(QIODevice::ReadOnly);
		//sloopServerFile.open(QIODevice::ReadOnly);
		//multiHostClientFile.open(QIODevice::ReadOnly);
		//multiHostServerFile.open(QIODevice::ReadOnly);

		if (!eolServerFile.open(QIODevice::ReadOnly)) {
		   qDebug("Could not open server certificate file: ");
		   qDebug(eolServerFile.fileName().toStdString().c_str());
                   return;
                }

		//QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, QByteArray("shiraz"));
		QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, QByteArray("client"));
		//QSslKey key(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey);
		QSslCertificate clientCert(&clientFile);

		qDebug(clientCert.effectiveDate().toString().toStdString().c_str());
		qDebug(clientCert.expiryDate().toString().toStdString().c_str());
		qDebug(clientCert.subjectInfo(QSslCertificate::Organization).toStdString().c_str());
		qDebug(clientCert.subjectInfo(QSslCertificate::CommonName).toStdString().c_str());
		qDebug(clientCert.subjectInfo(QSslCertificate::LocalityName).toStdString().c_str());
		qDebug(clientCert.subjectInfo(QSslCertificate::OrganizationalUnitName).toStdString().c_str());
		qDebug(clientCert.subjectInfo(QSslCertificate::CountryName).toStdString().c_str());
		qDebug(clientCert.subjectInfo(QSslCertificate::StateOrProvinceName).toStdString().c_str());
		//QSslCertificate serverCert(&serverFile);
		//QSslCertificate newClientCert(&newClientFile);
		//QSslCertificate newServerCert(&newServerFile);
		//QSslCertificate tikalClientCert(&tikalClientFile);
		//QSslCertificate tikalServerCert(&tikalServerFile);
		//QSslCertificate shirazClientCert(&shirazClientFile);
		//QSslCertificate shirazServerCert(&shirazServerFile);
		//QSslCertificate dropletClientCert(&dropletClientFile);
		//QSslCertificate dropletServerCert(&dropletServerFile);
		//QSslCertificate sloopClientCert(&sloopClientFile);
		//QSslCertificate sloopServerCert(&sloopServerFile);
		//QSslCertificate multiHostClientCert(&multiHostClientFile);
		//QSslCertificate multiHostServerCert(&multiHostServerFile);

		QSslCertificate eolServerCert(&eolServerFile);

		sslSocket_->setPrivateKey(key);
		//sslSocket_->setLocalCertificate(newClientCert);

		//sslSocket_->setLocalCertificate(multiHostClientCert);
		//sslSocket_->setLocalCertificate(shirazClientCert);
		sslSocket_->setLocalCertificate(clientCert);


		if (!sslSocket_->localCertificate().isValid()) {
			qDebug("Invalid client certificate");
			qDebug(" Is it out of date?  ExpiryDate:");
                        qDebug(sslSocket_->localCertificate().expiryDate().toString().toStdString().c_str());
                        
			return;
		}

		QMultiMap<QSsl::AlternateNameEntryType, QString> alternates = sslSocket_->localCertificate().alternateSubjectNames();
		if (alternates.isEmpty()) {
			qDebug("No alternates in client certificate.");
		} else {
			qDebug() << "Subject Alternate Names for client:\n" << alternates;
		}

		//QSslError error(QSslError::SelfSignedCertificate, serverCert);
		//QSslError newError(QSslError::SelfSignedCertificate, newServerCert);
		//QSslError tikalError(QSslError::SelfSignedCertificate, tikalServerCert);
		//QSslError shirazError(QSslError::SelfSignedCertificate, shirazServerCert);
		//QSslError dropletError(QSslError::SelfSignedCertificate, dropletServerCert);
		//QSslError sloopError(QSslError::SelfSignedCertificate, sloopServerCert);
		QSslError eolError(QSslError::SelfSignedCertificate, eolServerCert);

		QList<QSslError> expectedSslErrors;
		expectedSslErrors.append(eolError);
		//expectedSslErrors.append(newError);
		//expectedSslErrors.append(tikalError);
		//expectedSslErrors.append(shirazError);
		//expectedSslErrors.append(dropletError);
		//expectedSslErrors.append(sloopError);
		//expectedSslErrors.append(eolError);

		// Server connection to eol-rt-data is hardcoded here
		// Ignore only self-signed error on approved certificates during SSL connection
		//sslSocket_->connectToHostEncrypted("localhost", 54545);
		sslSocket_->ignoreSslErrors(expectedSslErrors);
		sslSocket_->connectToHostEncrypted("eol-rt-data.guest.ucar.edu", 80);

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
	//qDebug("Proxy::clientConnected");
	QString host = sslSocket_->peerAddress().toString();
	connection_->setText(tr("Connected to %1 at port %2.")
							.arg(host).arg(sslSocket_->peerPort()));
	connection_->show();
	quitButton_->show();
}

void Proxy::clientEncrypted()
{
	//qDebug("Proxy::clientEncrypted");
	QString host = sslSocket_->peerAddress().toString();
	connection_->setText(tr("Connected and encrypted to %1 at port %2.")
							.arg(host).arg(sslSocket_->peerPort()));
	connection_->show();
	quitButton_->show();
}

void Proxy::clientDisconnected()
{
	//qDebug("Proxy::clientDisconnected");
	QString host = sslSocket_->peerAddress().toString();
	connection_->setText(tr("Disconnected from %1.").arg(host));
	connection_->show();
}

void Proxy::readMode()
// Data sent over the SslSocket's connection should always be either the multiple instrument
// host error message or datagram status packets from the instrument
{
	//qDebug("Proxy::readMode");
	qint64 available = sslSocket_->bytesAvailable();
	QByteArray newMessage = sslSocket_->read(available);
	QString clientErrorString(tr("This instrument is already in use. Reconnect with new key."));

	if (newMessage == clientErrorString) {
		status_->setText(clientErrorString);
		sslSocket_->disconnectFromHost();
	} else {
		status_->setWordWrap(true);
		status_->setText(tr("New message from server received:\n").append(newMessage));
		//ILOG << newMessage.data();
	}
	status_->show();
}

void Proxy::sendMode()
{
	//qDebug("Proxy::sendMode");
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
	//qDebug("Proxy::sendToServer");
	while (udpInput_->hasPendingDatagrams()) {
		QByteArray newDatagram;
		newDatagram.resize(udpInput_->pendingDatagramSize());
		udpInput_->readDatagram(newDatagram.data(), newDatagram.size());

		sslSocket_->write(newDatagram.data(), newDatagram.size());
	}
}

void Proxy::switchHostMode()
{
	//qDebug("Proxy::switchHostMode");
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
	//qDebug("Proxy::displayError(single)");
	switch (socketError) {
	case QAbstractSocket::RemoteHostClosedError:
                QMessageBox::information(this, tr("Error"),
                             tr("The remote host closed the interface"));
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
	//qDebug("Proxy::displayError(list)");
	QString errorList;
	for (int i = 0; i < errors.size(); ++i) {
		errorList.append(tr("Error %1: %2.\n").arg(i+1).arg(errors[i].errorString()));
	}
	QMessageBox::information(this, tr("SSL Error"), errorList);
}

