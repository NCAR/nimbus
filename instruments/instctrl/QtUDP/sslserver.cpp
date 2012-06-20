#include "sslserver.h"

SslServer::SslServer(QWidget *parent)
	: QDialog(parent), sslServer_(0)
{
	status_ = new QLabel(tr("Enter port to listen for TCP connections:"));
	connection_ = new QLabel;
	connection_->hide();
	portLabel_ = new QLabel(tr("Port:"));
	port_ = new QComboBox;
	port_->setEditable(true);
	uniquePorts_ = new QSet<QString>;

	connectButton_ = new QPushButton(tr("Listen"));
	sendButton_ = new QPushButton(tr("Send"));
	sendButton_->hide();
	writeButton_ = new QPushButton(tr("Write new message"));
	writeButton_->hide();
	changePortButton_ = new QPushButton(tr("Change Port"));
	changePortButton_->hide();
	message_ = new QTextEdit;
	message_->hide();

	sslServer_ = new QSslServer(this);

	connect(connectButton_, SIGNAL(clicked()), this, SLOT(openSession()));
	connect(sslServer_, SIGNAL(newConnection()), this, SLOT(connectToClient()));
	connect(sendButton_, SIGNAL(clicked()), this, SLOT(sendMessage()));
	connect(writeButton_, SIGNAL(clicked()), this, SLOT(sendMode()));
	connect(changePortButton_, SIGNAL(clicked()), this, SLOT(switchPorts()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(changePortButton_);
	buttonLayout->addWidget(connectButton_);
	buttonLayout->addWidget(sendButton_);
	buttonLayout->addWidget(writeButton_);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(connection_, 0, 0, 1, 2);
	mainLayout->addWidget(status_, 1, 0, 1, 2);
	mainLayout->addWidget(portLabel_, 2, 0);
	mainLayout->addWidget(port_, 2, 1);
	mainLayout->addWidget(message_, 3, 0, 1, 2);
	mainLayout->addLayout(buttonLayout, 4, 0, 1, 2);
	setLayout(mainLayout);
	setWindowTitle(tr("SSL Server"));
}

void SslServer::openSession()
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

		// Listen on all network interfaces
		bool validSession = sslServer_->listen(QHostAddress::Any, portNumber);

		if (!validSession) {
			status_->setText(tr("Unable to start the server: %1.")
								.arg(sslServer_->errorString()));
			return;
		}

		status_->setText(tr("The server is listening on port %1 at address %2.")
							.arg(sslServer_->serverPort())
							.arg(sslServer_->serverAddress().toString()));

		connectButton_->hide();
		portLabel_->hide();
		port_->hide();
		changePortButton_->show();
	}
}

void SslServer::connectToClient()
{
	status_->setText(tr("New client available. Initiating handshake..."));
	currentSocket_ = sslServer_->nextPendingConnection();

	connect(currentSocket_, SIGNAL(connected()), this, SLOT(clientConnected()));
	connect(currentSocket_, SIGNAL(encrypted()), this, SLOT(clientEncrypted()));
	connect(currentSocket_, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	connect(currentSocket_, SIGNAL(encrypted()), this, SLOT(sendMode()));
	connect(currentSocket_, SIGNAL(readyRead()), this, SLOT(readMode()));
}

void SslServer::clientConnected()
{
	connection_->setText(tr("Client connected."));
	connection_->show();
}

void SslServer::clientEncrypted()
{
	if (currentSocket_->peerCertificate().isNull()) {
		connection_->setText(tr("Client connected and encrypted, but certificate is null."));
	} else {
		QString subjectInfo;
		QString issuerInfo;

		//Fetchin the subject info
		subjectInfo.append(currentSocket_->peerCertificate().issuerInfo(QSslCertificate::Organization));
		subjectInfo.append(currentSocket_->peerCertificate().issuerInfo(QSslCertificate::CommonName));
		//Fetching effective and expiry dates
		subjectInfo.append(currentSocket_->peerCertificate().effectiveDate().toString());
		subjectInfo.append(currentSocket_->peerCertificate().expiryDate().toString());
		//Fetching the serial number
		subjectInfo.append(currentSocket_->peerCertificate().serialNumber());

		//Fetchin the issuer info
		issuerInfo.append(currentSocket_->peerCertificate().issuerInfo(QSslCertificate::Organization));
		issuerInfo.append(currentSocket_->peerCertificate().issuerInfo(QSslCertificate::CommonName));
		//Fetching effective and expiry dates
		issuerInfo.append(currentSocket_->peerCertificate().effectiveDate().toString());
		issuerInfo.append(currentSocket_->peerCertificate().expiryDate().toString());
		//Fetching the serial number
		issuerInfo.append(currentSocket_->peerCertificate().serialNumber());

		connection_->setText(tr("Client connected and encrypted.\nSubject Info: %1\nIssuer Info: %2")
								.arg(subjectInfo).arg(issuerInfo));
	}
	connection_->show();
}

void SslServer::clientDisconnected()
{
	connection_->setText(tr("Client disconnected."));
	connection_->show();
}

void SslServer::sendMode()
{
	writeButton_->hide();
	status_->setText(tr("Write message below:"));
	message_->show();
	sendButton_->show();
}

void SslServer::sendMessage()
{
	status_->setWordWrap(false);
	connection_->hide();
	message_->hide();
	sendButton_->hide();

	QString portString = port_->currentText();
	int portNumber = portString.toInt();

	QByteArray block;
	block.append(message_->toPlainText());
	currentSocket_->write(block);

	status_->setText(tr("Message sent to client over port %1.")
						.arg(portNumber));
	message_->clear();
	writeButton_->show();
}

void SslServer::readMode()
{
	qint64 available = currentSocket_->bytesAvailable();
	QByteArray newMessage = currentSocket_->read(available);
	status_->setWordWrap(true);
	status_->setText(tr("New message from client received:\n").append(newMessage));
}

void SslServer::switchPorts()
{
	status_->setWordWrap(false);
	changePortButton_->hide();
	sendButton_->hide();
	writeButton_->hide();
	message_->hide();

	sslServer_->close();

	status_->setText(tr("Server closed. Listen on new port:"));
	portLabel_->show();
	port_->show();
	connectButton_->show();
}

