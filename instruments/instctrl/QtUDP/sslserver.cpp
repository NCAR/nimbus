#include "sslserver.h"

SslServer::SslServer(QWidget *parent)
	: QDialog(parent), sslServer_(0)
{
	status_ = new QLabel(tr("Enter port to listen for TCP connections:"));
	connection_ = new QLabel;
	connection_->hide();
	connectedClient_ = new QLabel;
	connectedClient_->hide();
	portLabel_ = new QLabel(tr("Port:"));
	port_ = new QComboBox;
	port_->setEditable(true);
	uniquePorts_ = new QSet<QString>;
	clientsLabel_ = new QLabel(tr("Clients:"));
	clientsLabel_->hide();
	clients_ = new QComboBox;
	clients_->hide();

	connectButton_ = new QPushButton(tr("Listen"));
	sendButton_ = new QPushButton(tr("Send"));
	sendButton_->hide();
	sendToClientButton_ = new QPushButton(tr("Send to client"));
	sendToClientButton_->hide();
	broadcastButton_ = new QPushButton(tr("Broadcast"));
	broadcastButton_->hide();
	writeButton_ = new QPushButton(tr("Write new message"));
	writeButton_->hide();
	changePortButton_ = new QPushButton(tr("Change Port"));
	changePortButton_->hide();
	quitButton_ = new QPushButton(tr("Quit"));
	quitButton_->hide();
	message_ = new QTextEdit;
	message_->hide();

	sslServer_ = new QSslServer(this);

	connect(connectButton_, SIGNAL(clicked()), this, SLOT(openSession()));
	connect(sslServer_, SIGNAL(newConnection()), this, SLOT(connectToClient()));
	connect(sendToClientButton_, SIGNAL(clicked()), this, SLOT(sendToClient()));
	connect(sendButton_, SIGNAL(clicked()), this, SLOT(sendMessage()));
	connect(broadcastButton_, SIGNAL(clicked()), this, SLOT(broadcastMessage()));
	connect(writeButton_, SIGNAL(clicked()), this, SLOT(sendMode()));
	connect(changePortButton_, SIGNAL(clicked()), this, SLOT(switchPorts()));
	connect(quitButton_, SIGNAL(clicked()), this, SLOT(quitSession()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(changePortButton_);
	buttonLayout->addWidget(connectButton_);
	buttonLayout->addWidget(sendButton_);
	buttonLayout->addWidget(sendToClientButton_);
	buttonLayout->addWidget(broadcastButton_);
	buttonLayout->addWidget(writeButton_);
	buttonLayout->addWidget(quitButton_);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(connection_, 0, 0, 1, 2);
	mainLayout->addWidget(connectedClient_, 1, 0, 1, 2);
	mainLayout->addWidget(status_, 2, 0, 1, 2);
	mainLayout->addWidget(portLabel_, 3, 0);
	mainLayout->addWidget(port_, 3, 1);
	mainLayout->addWidget(clientsLabel_, 4, 0);
	mainLayout->addWidget(clients_, 4, 1);
	mainLayout->addWidget(message_, 5, 0, 1, 2);
	mainLayout->addLayout(buttonLayout, 6, 0, 1, 2);
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
	QSslSocket *newClient = sslServer_->nextPendingConnection();

	connect(newClient, SIGNAL(connected()), this, SLOT(clientConnected()));
	connect(newClient, SIGNAL(encrypted()), this, SLOT(clientEncrypted()));
	connect(newClient, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	connect(newClient, SIGNAL(encrypted()), this, SLOT(sendMode()));
	connect(newClient, SIGNAL(readyRead()), this, SLOT(addClientName()));
}

void SslServer::addClientName()
{
	QSslSocket *newClient = qobject_cast<QSslSocket *>(sender());

	qint64 available = newClient->bytesAvailable();
	QByteArray name = newClient->read(available);
	QString clientName(name);

	clients_->addItem(clientName);
	connectedSockets_.insert(clientName, newClient);

	disconnect(newClient, SIGNAL(readyRead()), this, SLOT(addClientName()));
	connect(newClient, SIGNAL(readyRead()), this, SLOT(readMode()));

	connectedClient_->setText(tr("Client Name: \"%1\" ").arg(clientName));
	connectedClient_->show();
}

void SslServer::clientConnected()
{
	connection_->setText(tr("Client connected."));
	connection_->show();
	quitButton_->show();
}

void SslServer::clientEncrypted()
{
	QSslSocket *currentSocket = qobject_cast<QSslSocket *>(sender());
	if (!currentSocket)
		return;

	if (currentSocket->peerCertificate().isNull()) {
		connection_->setText(tr("Client connected and encrypted, but peer certificate is null."));
	} else {
		QString subjectInfo;
		QString issuerInfo;

		//Fetchin the subject info
		subjectInfo.append(currentSocket->peerCertificate().issuerInfo(QSslCertificate::Organization));
		subjectInfo.append(currentSocket->peerCertificate().issuerInfo(QSslCertificate::CommonName));
		//Fetching effective and expiry dates
		subjectInfo.append(currentSocket->peerCertificate().effectiveDate().toString());
		subjectInfo.append(currentSocket->peerCertificate().expiryDate().toString());
		//Fetching the serial number
		subjectInfo.append(currentSocket->peerCertificate().serialNumber());

		//Fetchin the issuer info
		issuerInfo.append(currentSocket->peerCertificate().issuerInfo(QSslCertificate::Organization));
		issuerInfo.append(currentSocket->peerCertificate().issuerInfo(QSslCertificate::CommonName));
		//Fetching effective and expiry dates
		issuerInfo.append(currentSocket->peerCertificate().effectiveDate().toString());
		issuerInfo.append(currentSocket->peerCertificate().expiryDate().toString());
		//Fetching the serial number
		issuerInfo.append(currentSocket->peerCertificate().serialNumber());

		connection_->setText(tr("Client connected and encrypted.\nSubject Info: %1\nIssuer Info: %2")
								.arg(subjectInfo).arg(issuerInfo));
	}
	connection_->show();
	quitButton_->show();
}

void SslServer::clientDisconnected()
{
	QSslSocket *currentSocket = qobject_cast<QSslSocket *>(sender());
	if (!currentSocket)
		return;

	for (QMap<QString, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		if (currentSocket == i.value()) {
			int clientIndex = clients_->findText(i.key());
			clients_->removeItem(clientIndex);
			connection_->setText(tr("Client \"%1\" disconnected.").arg(i.key()));
			connectedSockets_.erase(i);
			break;
		}
	}

	currentSocket->deleteLater();
	connection_->show();
}

void SslServer::sendMode()
{
	writeButton_->hide();
	status_->setText(tr("Write message below:"));
	message_->show();
	sendToClientButton_->show();
	broadcastButton_->show();
}

void SslServer::sendToClient()
{
	connection_->hide();
	connectedClient_->hide();
	broadcastButton_->hide();
	sendToClientButton_->hide();
	status_->setText(tr("Choose client to send message to:"));
	clientsLabel_->show();
	clients_->show();
	sendButton_->show();
}

void SslServer::sendMessage()
{
	status_->setWordWrap(false);
	connection_->hide();
	connectedClient_->hide();
	clientsLabel_->hide();
	clients_->hide();
	message_->hide();
	sendButton_->hide();

	QString portString = port_->currentText();
	int portNumber = portString.toInt();

	QString clientName = clients_->currentText();

	QByteArray block;
	block.append(message_->toPlainText());
	connectedSockets_.value(clientName)->write(block);

	status_->setText(tr("Message sent to \"%1\" over port %2.")
						.arg(clientName).arg(portNumber));
	message_->clear();
	writeButton_->show();
}

void SslServer::broadcastMessage()
{
	status_->setWordWrap(false);
	connection_->hide();
	connectedClient_->hide();
	message_->hide();
	sendButton_->hide();
	sendToClientButton_->hide();
	broadcastButton_->hide();

	QString portString = port_->currentText();
	int portNumber = portString.toInt();

	QByteArray block;
	block.append(message_->toPlainText());

	foreach(QSslSocket *client, connectedSockets_) {
		client->write(block);
	}

	status_->setText(tr("Message sent to client over port %1.")
						.arg(portNumber));
	message_->clear();
	writeButton_->show();
}

void SslServer::readMode()
{
	connection_->hide();
	connectedClient_->hide();

	QSslSocket *currentSocket = qobject_cast<QSslSocket *>(sender());
	if (!currentSocket)
		return;

	qint64 available = currentSocket->bytesAvailable();
	QByteArray newMessage = currentSocket->read(available);
	status_->setWordWrap(true);

	for (QMap<QString, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		if (currentSocket == i.value()) {
			status_->setText(tr("New message from client \"%1\" received:\n").arg(i.key()).append(newMessage));
		}
	}
}

void SslServer::switchPorts()
{
	status_->setWordWrap(false);
	connectedClient_->hide();
	clientsLabel_->hide();
	clients_->hide();
	changePortButton_->hide();
	sendButton_->hide();
	sendToClientButton_->hide();
	broadcastButton_->hide();
	writeButton_->hide();
	message_->hide();

	for (QMap<QString, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		i.value()->deleteLater();
	}
	connectedSockets_.clear();
	sslServer_->close();
	clients_->clear();

	if (connectedSockets_.empty()) {
		connection_->setText(tr("Clients have been disconnected."));
	}
	status_->setText(tr("Server closed. Listen on new port:"));
	portLabel_->show();
	port_->show();
	connectButton_->show();
}

void SslServer::quitSession()
{
	for (QMap<QString, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		i.value()->deleteLater();
	}
	close();
}

