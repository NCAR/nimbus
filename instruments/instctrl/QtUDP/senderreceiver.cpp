#include "senderreceiver.h"
#include <QtNetwork/QHostAddress>
#include <QtCore>

SenderReceiver::SenderReceiver(QWidget *parent)
	: QDialog(parent)
{
	udpSocket_ = new QUdpSocket();
	readButton_ = new QPushButton (tr("&Read"));
	writeButton_ = new QPushButton(tr("&Write"));
	singleSendButton_ = new QPushButton(tr("&Send"));
	multiSendButton_ = new QPushButton(tr("&Broadcast"));
	changePortButton_ = new QPushButton(tr("&Switch Port"));
	singleSendButton_->hide();
	multiSendButton_->hide();
	changePortButton_->hide();
	mode_ = new QLabel(tr("Enter port number and choose mode:"));
	connect_ = new QLabel(tr("Connection..."));
	connect_->hide();
	error_ = new QLabel(tr("Error:"));
	error_->hide();

	portLabel_ = new QLabel(tr("Port:"));
	addressLabel_ = new QLabel(tr("Address:"));
	messageLabel_ = new QLabel(tr("Message:"));
	addressLabel_->hide();
	messageLabel_->hide();
	uniqueAddresses_ = new QSet<QString>;
	uniquePorts_ = new QSet<QString>;
	port_ = new QComboBox;
	port_->setEditable(true);
	address_ = new QComboBox;
	address_->hide();
	message_ = new QTextEdit;
	message_->hide();

	singleSend_ = new QAction(tr("Send to single address"), this);
	broadcastSend_ = new QAction(tr("Broadcast to all addresses"), this);
	writeMode_ = new QPushButton(tr("Select send mode"));
	QMenu *modeMenu = new QMenu;
	modeMenu->addAction(singleSend_);
	modeMenu->addAction(broadcastSend_);
	writeMode_->setMenu(modeMenu);
	writeMode_->hide();

	connect(readButton_, SIGNAL(clicked()), this, SLOT(readMode()));
	connect(udpSocket_, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	connect(writeButton_, SIGNAL(clicked()), this, SLOT(writeMode()));
	connect(singleSend_, SIGNAL(triggered()), this, SLOT(writeToOne()));
	connect(broadcastSend_, SIGNAL(triggered()), this, SLOT(writeToAll()));
	connect(singleSendButton_, SIGNAL(clicked()), this, SLOT(writeNewDatagram()));
	connect(multiSendButton_, SIGNAL(clicked()), this, SLOT(broadcastDatagrams()));
	connect(changePortButton_, SIGNAL(clicked()), this, SLOT(switchPortMode()));
	connect(udpSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorMessage()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(changePortButton_);
	buttonLayout->addWidget(readButton_);
	buttonLayout->addWidget(writeButton_);
	buttonLayout->addWidget(singleSendButton_);
	buttonLayout->addWidget(multiSendButton_);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(error_, 0, 0, 1, 2);
	mainLayout->addWidget(connect_, 1, 0, 1, 2);
	mainLayout->addWidget(mode_, 2, 0, 1, 2);
	mainLayout->addWidget(writeMode_, 3, 0, 1, 2);
	mainLayout->addWidget(portLabel_, 4, 0);
	mainLayout->addWidget(port_, 4, 1);
	mainLayout->addWidget(addressLabel_, 5, 0);
	mainLayout->addWidget(address_, 5, 1);
	mainLayout->addWidget(messageLabel_, 6, 0);
	mainLayout->addWidget(message_, 6, 1);
	mainLayout->addLayout(buttonLayout, 7, 0, 1, 2);
	setLayout(mainLayout);
	setWindowTitle(tr("Sender/Receiver"));
}

void SenderReceiver::readMode()
{
	connect_->hide();
	QString portString = port_->currentText();
	bool validPort;
	int portNumber = portString.toInt(&validPort);

	if (!validPort || portNumber > 65535 || portNumber < 0) {
		mode_->setText(tr("Invalid port number. Retry:"));
		return;
	} else {
		readButton_->hide();
		port_->hide();
		port_->setEditable(false);
		portLabel_->hide();

		// If entered port is new, must add it to the combobox and
		// set it as the current item
		if (!uniquePorts_->contains(portString)) {
			uniquePorts_->insert(portString);
			port_->addItem(portString);
			int newPortIndex = port_->findText(portString);
			port_->setCurrentIndex(newPortIndex);
		}

		if (udpSocket_->localPort() != portNumber) {
			udpSocket_->disconnectFromHost();
			bool connection = udpSocket_->bind(portNumber);
			connect_->show();
			if (!connection) {
				connect_->setText(tr("Connection to new port unsuccessful. Retry:"));
				mode_->hide();
				changePortButton_->show();
				writeButton_->hide();
				return;
			} else connect_->setText(tr("Connection to new port successful."));
		}

		mode_->setText(tr("Reading at port %1...").arg(portNumber));
		writeButton_->show();
		changePortButton_->show();
	}
}

void SenderReceiver::readPendingDatagrams()
{
	while (udpSocket_->hasPendingDatagrams()) {
		QByteArray datagram;
		QHostAddress *sender = new QHostAddress();
		datagram.resize(udpSocket_->pendingDatagramSize());
		udpSocket_->readDatagram(datagram.data(), datagram.size(), sender);
		QString address = sender->toString();

		// Filter out messages not sent by this program
		if (!(strncmp(datagram.data(), "Fawaz: ", 7) == 0)) {
			return;
		} else {
			connect_->hide();
			mode_->setWordWrap(true);
			mode_->setText(tr("Message received at port %1\nFrom address %2:\n\n\"%3\"")
								.arg(port_->currentText())
								.arg(address)
								.arg(datagram.data()));
			readButton_->show();
			writeButton_->show();
		}
	}
}

void SenderReceiver::writeMode()
{
	connect_->hide();
	mode_->setWordWrap(false);
	QString portString = port_->currentText();
	bool validPort;
	int portNumber = portString.toInt(&validPort);

	if (!validPort || portNumber > 65535 || portNumber < 0) {
		mode_->setText(tr("Invalid port number. Retry:"));
		return;
	} else {
		mode_->setText(tr("Choose write mode and write message below:"));
		port_->show();
		port_->setEditable(true);
		portLabel_->show();
		readButton_->hide();
		writeButton_->hide();

		// If entered port is new, must add it to the combobox and
		// set it as the current item
		if (!uniquePorts_->contains(portString)) {
			uniquePorts_->insert(portString);
			port_->addItem(portString);
			int newPortIndex = port_->findText(portString);
			port_->setCurrentIndex(newPortIndex);
		}

		if (udpSocket_->localPort() != portNumber) {
			udpSocket_->disconnectFromHost();
			bool connection = udpSocket_->bind(portNumber);
			connect_->show();
			if (!connection) {
				connect_->setText(tr("Connection to new port unsuccessful. Retry:"));
				mode_->hide();
				changePortButton_->show();
				return;
			} else connect_->setText(tr("Connection to new port successful"));
		}

		writeMode_->show();
		messageLabel_->show();
		message_->show();
		changePortButton_->show();
	}
}

void SenderReceiver::writeToOne()
{
	connect_->hide();
	mode_->setText(tr("Write address and message below:"));
	addressLabel_->show();
	address_->show();
	address_->setEditable(true);
	singleSendButton_->show();
	multiSendButton_->hide();
}

void SenderReceiver::writeToAll()
{
	connect_->hide();
	mode_->setText(tr("Write message to everyone at port %1:")
						.arg(port_->currentText()));
	multiSendButton_->show();
	addressLabel_->hide();
	address_->hide();
	singleSendButton_->hide();
}

void SenderReceiver::writeNewDatagram()
{
	QString data = message_->toPlainText();
	QString portString = port_->currentText();
	int portNumber = portString.toInt();
	QString address = address_->currentText();
	QHostAddress ip;
	bool validAddress = ip.setAddress(address);

	if (!validAddress) {
		mode_->setText(tr("Invalid IP address. Retry:"));
		return;
	} else {
		QByteArray newData("Fawaz: ");
		newData.append(data);

		// send message to specified address
		udpSocket_->writeDatagram(newData.data(), newData.size(), ip, portNumber);

		mode_->setText(tr("Message sent to %1 at port %2.")
							.arg(address).arg(portString));
		singleSendButton_->hide();
		addressLabel_->hide();
		address_->hide();
		address_->setEditable(false);

		if (!uniqueAddresses_->contains(address)) {
			uniqueAddresses_->insert(address);
			address_->addItem(address);
		}

		readButton_->show();
		writeButton_->show();
		writeMode_->hide();
		messageLabel_->hide();
		message_->hide();
		message_->clear();
		portLabel_->hide();
		port_->hide();
	}
}

void SenderReceiver::broadcastDatagrams()
{
	QString data = message_->toPlainText();
	QString portString = port_->currentText();
	int portNumber = portString.toInt();

	QByteArray newData("Fawaz: ");
	newData.append(data);

	// send message to broadcast address; received by all devices on local network
	udpSocket_->writeDatagram(newData.data(), newData.size(),
								QHostAddress::Broadcast, portNumber);

	mode_->setText(tr("Message broadcasted at port %1.").arg(port_->currentText()));
	multiSendButton_->hide();
	readButton_->show();
	writeButton_->show();
	writeMode_->hide();
	messageLabel_->hide();
	message_->hide();
	message_->clear();
	portLabel_->hide();
	port_->hide();
}

void SenderReceiver::switchPortMode()
{
	mode_->show();
	mode_->setWordWrap(false);
	mode_->setText(tr("Set port number:"));
	portLabel_->show();
	port_->show();
	port_->setEditable(true);
	readButton_->show();
	writeButton_->show();
	connect_->hide();
	singleSendButton_->hide();
	multiSendButton_->hide();
	changePortButton_->hide();
	addressLabel_->hide();
	messageLabel_->hide();
	address_->hide();
	message_->hide();
	writeMode_->hide();
}

void SenderReceiver::errorMessage()
{
	error_->setText(tr("Error: %1").arg(udpSocket_->errorString()));
	error_->show();
}

