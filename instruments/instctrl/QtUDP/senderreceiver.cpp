#include "senderreceiver.h"
#include <QtNetwork/QHostAddress>
#include <QtCore>

static const int port = 45454;

SenderReceiver::SenderReceiver(QWidget *parent)
	: QDialog(parent)
{
	udpSocket_ = new QUdpSocket();
	writeButton_ = new QPushButton(tr("&Write"));
	sendButton_ = new QPushButton(tr("&Send"));
//	readButton_ = new QPushButton(tr("&Read"));
	mode_ = new QLabel(tr("Reading..."));
	message_ = new QTextEdit;
	message_->hide();
//	address_ = new QLineEdit;
//	address_->hide();
	sendButton_->hide();

	QHostAddress host = QHostAddress::Any;
	udpSocket_->bind(host, port);

	connect(udpSocket_, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
//	connect(readButton_, SIGNAL(clicked()), this, SLOT(readPendingDatagrams()));
	connect(writeButton_, SIGNAL(clicked()), this, SLOT(writeMode()));
	connect(sendButton_, SIGNAL(clicked()), this, SLOT(writeNewDatagram()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
//	buttonLayout->addWidget(readButton_);
	buttonLayout->addWidget(writeButton_);
	buttonLayout->addWidget(sendButton_);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mode_);
//	mainLayout->addWidget(address_);
	mainLayout->addWidget(message_);
	mainLayout->addLayout(buttonLayout);
	setLayout(mainLayout);
	setWindowTitle(tr("Sender/Receiver"));
}

void SenderReceiver::readPendingDatagrams()
{
	while (udpSocket_->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(udpSocket_->pendingDatagramSize());
		udpSocket_->readDatagram(datagram.data(), datagram.size());

		if (!(strncmp(datagram.data(), "Fawaz: ", 7) == 0)) {
			return;
		} else {
			mode_->setText(tr("Message received: \"%1\"")
								.arg(datagram.data()));
		}
	}
}

void SenderReceiver::writeMode()
{
	mode_->setText(tr("Write message below:"));
//	address_->show();
	message_->show();
	writeButton_->hide();
	sendButton_->show();
}

void SenderReceiver::writeNewDatagram()
{
//	QString address = address_->text();
//	QHostAddress ip = QHostAddress(address);
	QString data = message_->toPlainText();

	QByteArray newData("Fawaz: ");
	newData.append(data);
	udpSocket_->writeDatagram(newData.data(), newData.size(),
								QHostAddress::Broadcast, port);

	mode_->setText(tr("Message sent."));
	sendButton_->hide();
	writeButton_->show();
//	address_->hide();
	message_->hide();
//	address_->clear();
	message_->clear();
}

