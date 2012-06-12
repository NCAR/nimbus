#include "sslclient.h"

SslClient::SslClient(QWidget *parent)
	: QDialog(parent)
{
	status_ = new QLabel(tr("Enter port and server name to connect:"));
	portLabel_ = new QLabel(tr("Port:"));
	port_ = new QComboBox;
	port_->setEditable(true);
	uniquePorts_ = new QSet<QString>;
	hostLabel_ = new QLabel(tr("Host Name:"));
	hostName_ = new QLineEdit;
	message_ = new QTextEdit;
	message_->hide();

	connectButton_ = new QPushButton(tr("Connect"));
	switchButton_ = new QPushButton(tr("Switch"));
	switchButton_->hide();
	sendButton_ = new QPushButton(tr("Send"));
	sendButton_->hide();

	sslSocket_ = new QSslSocket(this);

	connect(connectButton_, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(sslSocket_, SIGNAL(encrypted()), this, SLOT(sendMode()));
	connect(switchButton_, SIGNAL(clicked()), this, SLOT(switchHostMode()));
	connect(sslSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this,
			SLOT(displayError(QAbstractSocket::SocketError)));
	connect(sslSocket_, SIGNAL(sslErrors(const QList<QSslError> &)), this,
			SLOT(displayError(const QList<QSslError> &)));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(switchButton_);
	buttonLayout->addWidget(connectButton_);
	buttonLayout->addWidget(sendButton_);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(status_, 0, 0, 1, 2);
	mainLayout->addWidget(portLabel_, 1, 0);
	mainLayout->addWidget(port_, 1, 1);
	mainLayout->addWidget(hostLabel_, 2, 0);
	mainLayout->addWidget(hostName_, 2, 1);
	mainLayout->addWidget(message_, 3, 0, 1, 2);
	mainLayout->addLayout(buttonLayout, 4, 0, 1, 2);
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
		sslSocket_->connectToHostEncrypted(hostName_->text(), portNumber);
	}
}

void SslClient::sendMode()
{
	portLabel_->hide();
	port_->hide();
	hostLabel_->hide();
	hostName_->hide();
	connectButton_->hide();

	QString portString = port_->currentText();
	int portNumber = portString.toInt();

	status_->setText(tr("Connected to %1 on port %2.\n\nWrite message below:"));

	message_->show();
	sendButton_->show();
	switchButton_->show();
}

void SslClient::switchHostMode()
{
	switchButton_->hide();
	sendButton_->hide();

	status_->setText(tr("Enter new port and host:"));
	portLabel_->show();
	port_->show();
	hostLabel_->show();
	hostName_->show();
	connectButton_->show();
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
	}
}

void SslClient::displayError(const QList<QSslError> & errors)
{
	QString errorList;
	for (int i = 0; i < errors.size(); ++i) {
		errorList.append(tr("Error %1: %2.\n").arg(i).arg(errors[i].errorString()));
	}
	QMessageBox::information(this, tr("SSL Error"), errorList);
}

