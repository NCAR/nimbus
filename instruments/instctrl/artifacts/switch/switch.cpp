#include "switch.h"
#include "logx/Logging.h"

LOGGING("switch");

static const int udpPort = 54544;

Switch::Switch(QWidget *parent)
	: QDialog(parent), sslServer_(0)
{
	// Set up for symmetric decryption capability
           _init = new QCA::Initializer();
           if(!QCA::isSupported("aes128-cbc-pkcs7")) {
              QMessageBox error(this);
              error.setText("qca-ossl is not installed on this system\n  Exiting...");
              error.exec();
              exit(1);
           }

        // Set up the key and initialization vectors for the cipher
        //  Note: it would probably be best to have these in files shared
        //   between the switch on the ground and the switch on the aircraft.
        _key = new QCA::SymmetricKey(QCA::hexToArray(
                "22971df220ce5ade1f7fd7188e64e36ea5bd5e07a463dcd2dc9b8cc271afd6bbdd3dc54a334c2a1728237c1bd1d981902caaaeaa600f4d0ac38c62ee9e464d2822971df220ce5ade1f7fd7188e64e36ea5bd5e07a463dcd2dc9b8cc271afd6bbdd3dc54a334c2a1728237c1bd1d981902caaaeaa600f4d0ac38c62ee9e464d28"));
        _iv = new QCA::InitializationVector(QCA::hexToArray(
                "b35b2fd47af10dc72e0ab6c98a1d64825264233493f6f42caa93b0f90641ae59ac9b23f77de34bd639dc4b8a4259d32c2093860ac83e1a219b593e4b2fc83f37"));

        _cipher = new QCA::Cipher(QString("aes128"),QCA::Cipher::CBC,
                  // use Default padding, which is equivalent to PKCS7 for CBC
                      QCA::Cipher::DefaultPadding,
                  // this object will decrypt
                      QCA::Decode,
                      *_key, *_iv);

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
	chooseClientButton_ =  new QPushButton(tr("Show Datagram Log"));
	chooseClientButton_->hide();
	showDatagramsButton_ = new QPushButton(tr("Show"));
	showDatagramsButton_->hide();
	changePortButton_ = new QPushButton(tr("Change Port"));
	changePortButton_->hide();
	quitButton_ = new QPushButton(tr("Quit"));
	quitButton_->hide();

	sslServer_ = new QSslServer(this);
	udpSocket_ = new QUdpSocket();

	connect(connectButton_, SIGNAL(clicked()), this, SLOT(openSession()));
	connect(sslServer_, SIGNAL(newConnection()), this, SLOT(connectToClient()));
	connect(udpSocket_, SIGNAL(readyRead()), this, SLOT(sendToClient()));
	connect(chooseClientButton_, SIGNAL(clicked()), this, SLOT(chooseClient()));
	connect(showDatagramsButton_, SIGNAL(clicked()), this, SLOT(showDatagrams()));
	connect(changePortButton_, SIGNAL(clicked()), this, SLOT(switchPorts()));
	connect(quitButton_, SIGNAL(clicked()), this, SLOT(quitSession()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(changePortButton_);
	buttonLayout->addWidget(connectButton_);
	buttonLayout->addWidget(chooseClientButton_);
	buttonLayout->addWidget(showDatagramsButton_);
	buttonLayout->addWidget(quitButton_);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(connection_, 0, 0, 1, 2);
	mainLayout->addWidget(connectedClient_, 1, 0, 1, 2);
	mainLayout->addWidget(status_, 2, 0, 1, 2);
	mainLayout->addWidget(portLabel_, 3, 0);
	mainLayout->addWidget(port_, 3, 1);
	mainLayout->addWidget(clientsLabel_, 4, 0);
	mainLayout->addWidget(clients_, 4, 1);
	mainLayout->addLayout(buttonLayout, 5, 0, 1, 2);
	setLayout(mainLayout);
	setWindowTitle(tr("Switch"));
}

void Switch::openSession()
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

		udpSocket_->bind(udpPort);
		status_->setText(tr("The server is listening on port %1 at address %2.")
							.arg(sslServer_->serverPort())
							.arg(sslServer_->serverAddress().toString()));

		connectButton_->hide();
		portLabel_->hide();
		port_->hide();
		connection_->hide();
		changePortButton_->show();
		quitButton_->show();
	}
}

void Switch::connectToClient()
{
	status_->setText(tr("New client available. Initiating handshake..."));
	QSslSocket *newClient = sslServer_->nextPendingConnection();

	connect(newClient, SIGNAL(connected()), this, SLOT(clientConnected()));
	connect(newClient, SIGNAL(encrypted()), this, SLOT(clientEncrypted()));
	connect(newClient, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	connect(newClient, SIGNAL(readyRead()), this, SLOT(addClientName()));
}

void Switch::addClientName()
// First message sent by the proxy will be the instrument info; map the key to the socket
// pointer and add it to the list of connected sockets, then set the socket up to read
// messages from the connection normally.
{
	QSslSocket *newClient = qobject_cast<QSslSocket *>(sender());

	qint64 available = newClient->bytesAvailable();
	QByteArray clientInfo = newClient->read(available);
	QString clientString(clientInfo);
	QString clientName = clientString.section(',', 0, 0);
	QString portString = clientString.section(',', 1, 1);
	int clientPort = portString.toInt();
	QString addressString = clientString.section(',', 2, 2);
	QHostAddress clientAddress(addressString);
	QList<QByteArray> datagramList;

	for (QMap<ProxyClient, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		if (i.key().instKey_ == clientName) {
			QByteArray block;
			block.append(tr("This instrument is already in use. Reconnect with new key."));
			newClient->write(block);
			return;
		}
	}

	clients_->addItem(clientName);

	ProxyClient clientProfile;
	clientProfile.instKey_ = clientName;
	clientProfile.portNumber_ = clientPort;
	clientProfile.ipAddress_ = clientAddress;

	connectedSockets_.insert(clientProfile, newClient);
	udpLists_.insert(newClient, datagramList);

	disconnect(newClient, SIGNAL(readyRead()), this, SLOT(addClientName()));
	connect(newClient, SIGNAL(readyRead()), this, SLOT(readMode()));

	connectedClient_->setText(tr("Client Name: \"%1\"\nClient Port: \"%2\"\nClient IP: \"%3\"")
							.arg(clientName).arg(portString).arg(addressString));
	connectedClient_->show();
	chooseClientButton_->show();
}

void Switch::clientConnected()
{
	connection_->setText(tr("Client connected."));
	connection_->show();
	quitButton_->show();
}

void Switch::clientEncrypted()
{
	status_->hide();
	QSslSocket *currentSocket = qobject_cast<QSslSocket *>(sender());
	if (!currentSocket)
		return;

	QString host = currentSocket->peerAddress().toString();
	connection_->setText(tr("Connected and encrypted to %1 at port %2.")
							.arg(host).arg(currentSocket->localPort()));
	connection_->show();
	quitButton_->show();
}

void Switch::clientDisconnected()
{
	QSslSocket *currentSocket = qobject_cast<QSslSocket *>(sender());
	if (!currentSocket)
		return;

	for (QMap<ProxyClient, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		if (currentSocket == i.value()) {
			int clientIndex = clients_->findText(i.key().instKey_);
			clients_->removeItem(clientIndex);
			connection_->setText(tr("Client \"%1\" disconnected.").arg(i.key().instKey_));
			connectedSockets_.erase(i);

			currentSocket->deleteLater();
			connectedClient_->hide();
			connection_->show();
			break;
		}
	}
}

void Switch::chooseClient()
{
	connection_->hide();
	connectedClient_->hide();
	chooseClientButton_->hide();

	status_->setText(tr("Choose client to view their datagram log:"));

	status_->show();
	clientsLabel_->show();
	clients_->show();
	showDatagramsButton_->show();
}

void Switch::showDatagrams()
{
	showDatagramsButton_->hide();
	clientsLabel_->hide();
	clients_->hide();

	QString clientName = clients_->currentText();
	ProxyClient currentClient;
	for (QMap<ProxyClient, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		if (i.key().instKey_ == clientName) {
			currentClient.instKey_ = i.key().instKey_;
			currentClient.portNumber_ = i.key().portNumber_;
			currentClient.ipAddress_ = i.key().ipAddress_;
			break;
		}
	}
	QSslSocket * client = connectedSockets_.value(currentClient);
	QList<QByteArray> clientLog = udpLists_.value(client);
	for (QList<QByteArray>::iterator i = clientLog.begin(); i != clientLog.end(); ++i) {
		qDebug() << (*i).data();
	}

	status_->setText(tr("Datagram log of client \"%1\" printed.").arg(clientName));
	chooseClientButton_->show();
}

void Switch::readMode()
// Put read datagrams into the proxy sender's datagram list and write the datagram to ILOG
// (which will write it to Server Datagram Log)
{
	connection_->hide();
	connectedClient_->hide();

	QSslSocket *currentSocket = qobject_cast<QSslSocket *>(sender());
	if (!currentSocket)
		return;

	qint64 available = currentSocket->bytesAvailable();
	QByteArray newMessage = currentSocket->read(available);

	QMap<QSslSocket *, QList<QByteArray> >::iterator datagramDestination = udpLists_.find(currentSocket);
	datagramDestination.value().append(newMessage);

	ILOG << newMessage.data();

	for (QMap<ProxyClient, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		if (currentSocket == i.value()) {
			status_->setText(tr("New datagram from client \"%1\" received and stored.").arg(i.key().instKey_));
			status_->show();
		}
	}
}

void Switch::sendToClient()
// Sends datagrams received from the aircraft server to the respective clients.
// The first field in the datagram is the instrument key; use it to send the datagram to
// the right client. If there is no client with the datagram's instrument key, the
// datagram is sent to the error log for the server.
{
	while (udpSocket_->hasPendingDatagrams()) {
		connection_->hide();
		connectedClient_->hide();
		clientsLabel_->hide();
		clients_->hide();

		QCA::SecureArray encodedMsg;
		encodedMsg.resize(udpSocket_->pendingDatagramSize());
		udpSocket_->readDatagram(encodedMsg.data(), encodedMsg.size());

		QCA::SecureArray decodedMsg(_cipher->process(encodedMsg));

		if (!_cipher->ok()) { 
    			printf("Cipher failed\n");
		}
printf("Cipher succeeded!\n");

		QByteArray newDatagram(decodedMsg.data());

		QString datagram(newDatagram.data());
		QString clientName = datagram.section(',', 0, 0);
printf(" - DG:%s\n", datagram.toStdString().c_str());

		for (QMap<ProxyClient, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
			if (i.key().instKey_ == clientName) {
				i.value()->write(newDatagram);

				status_->setText(tr("Message sent to instrument\"%1\".").arg(clientName));
				status_->show();
				return;
			}
		}

		ELOG << newDatagram.data();
	}
}

void Switch::switchPorts()
{
	connectedClient_->hide();
	clientsLabel_->hide();
	clients_->hide();
	chooseClientButton_->hide();
	showDatagramsButton_->hide();
	changePortButton_->hide();

	// Clear all lists and disconnect all sockets before switching to a new port
	for (QMap<ProxyClient, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		i.value()->deleteLater();
	}
	connectedSockets_.clear();
	udpSocket_->disconnectFromHost();
	sslServer_->close();
	clients_->clear();

	if (connectedSockets_.empty()) {
		connection_->setText(tr("Clients have been disconnected."));
	}
	status_->setText(tr("Server closed. Listen on new port:"));
	status_->show();
	portLabel_->show();
	port_->show();
	connectButton_->show();
}

void Switch::quitSession()
{
	for (QMap<ProxyClient, QSslSocket *>::iterator i = connectedSockets_.begin(); i != connectedSockets_.end(); ++i) {
		i.value()->deleteLater();
	}
	close();
}

