#include "senderreceiver.h"
#include <QtNetwork/QHostAddress>
#include <QtCore>

static const int port = 45454;

SenderReceiver::SenderReceiver(QWidget *parent)
	: QDialog(parent)
{
	udpSocket_ = new QUdpSocket();
	writeButton_ = new QPushButton(tr("&Write"));
	singleSendButton_ = new QPushButton(tr("&Send"));
	multiSendButton_ = new QPushButton(tr("&Broadcast"));
	mode_ = new QLabel(tr("Reading..."));

	addressLabel_ = new QLabel(tr("Address:"));
	messageLabel_ = new QLabel(tr("Message:"));
	address_ = new QComboBox;
	address_->hide();
	addressLabel_->hide();
	message_ = new QTextEdit;
	message_->hide();
	messageLabel_->hide();
	singleSendButton_->hide();
	multiSendButton_->hide();

	singleSend_ = new QAction(tr("Send to single address"), this);
	broadcastSend_ = new QAction(tr("Broadcast to all addresses"), this);
	writeMode_ = new QPushButton(tr("Select send mode"));
	QMenu *modeMenu = new QMenu;
	modeMenu->addAction(singleSend_);
	modeMenu->addAction(broadcastSend_);
	writeMode_->setMenu(modeMenu);
	writeMode_->hide();

	QHostAddress host = QHostAddress::Any; // local network
	udpSocket_->bind(host, port);

	connect(udpSocket_, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	connect(writeButton_, SIGNAL(clicked()), this, SLOT(writeMode()));
	connect(singleSend_, SIGNAL(triggered()), this, SLOT(writeToOne()));
	connect(broadcastSend_, SIGNAL(triggered()), this, SLOT(writeToAll()));
	connect(singleSendButton_, SIGNAL(clicked()), this, SLOT(writeNewDatagram()));
	connect(multiSendButton_, SIGNAL(clicked()), this, SLOT(broadcastDatagrams()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(writeButton_);
	buttonLayout->addWidget(singleSendButton_);
	buttonLayout->addWidget(multiSendButton_);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mode_);
	mainLayout->addWidget(writeMode_);
	mainLayout->addWidget(addressLabel_);
	mainLayout->addWidget(address_);
	mainLayout->addWidget(messageLabel_);
	mainLayout->addWidget(message_);
	mainLayout->addLayout(buttonLayout);
	setLayout(mainLayout);
	resize(350, 300);
	setWindowTitle(tr("Sender/Receiver"));
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
			mode_->setWordWrap(true);
			mode_->setText(tr("Message received at address \"%1\":\n\n \"%2\"")
								.arg(address)
								.arg(datagram.data()));
		}
	}
}

void SenderReceiver::writeMode()
{
	mode_->setWordWrap(false);
	mode_->setText(tr("Choose mode and write message below:"));
	writeMode_->show();
	messageLabel_->show();
	message_->show();
	writeButton_->hide();
}

void SenderReceiver::writeToOne()
{
	mode_->setText(tr("Write address and message below:"));
	addressLabel_->show();
	address_->show();
	address_->setEditable(true);
	singleSendButton_->show();
	multiSendButton_->hide();
}

void SenderReceiver::writeToAll()
{
	mode_->setText(tr("Write message to everyone at port \"%1\":").arg(port));
	multiSendButton_->show();
	addressLabel_->hide();
	address_->hide();
	singleSendButton_->hide();
}

void SenderReceiver::writeNewDatagram()
{
	QString data = message_->toPlainText();
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
		udpSocket_->writeDatagram(newData.data(), newData.size(), ip, port);

		mode_->setText(tr("Message sent."));
		singleSendButton_->hide();
		addressLabel_->hide();
		address_->hide();
		address_->setEditable(false);
		address_->addItem(address);
		writeButton_->show();
		writeMode_->hide();
		messageLabel_->hide();
		message_->hide();
		message_->clear();
	}
}

void SenderReceiver::broadcastDatagrams()
{
	QString data = message_->toPlainText();

	QByteArray newData("Fawaz: ");
	newData.append(data);

	// send message to broadcast address; received by all network-attached hosts
	udpSocket_->writeDatagram(newData.data(), newData.size(),
								QHostAddress::Broadcast, port);

	mode_->setText(tr("Message broadcasted."));
	multiSendButton_->hide();
	writeButton_->show();
	writeMode_->hide();
	messageLabel_->hide();
	message_->hide();
	message_->clear();
}

