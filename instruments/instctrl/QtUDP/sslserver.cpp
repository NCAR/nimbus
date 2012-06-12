#include "sslserver.h"

SslServer::SslServer(QWidget *parent)
	: QDialog(parent), sslServer_(0)
{
	status_ = new QLabel(tr("Enter port to listen for TCP connections:"));
	portLabel_ = new QLabel(tr("Port:"));
	port_ = new QComboBox;
	port_->setEditable(true);
	uniquePorts_ = new QSet<QString>;

	connectButton_ = new QPushButton(tr("Listen"));
	sendButton_ = new QPushButton(tr("Send"));
	sendButton_->hide();
	changePortButton_ = new QPushButton(tr("Change Port"));
	changePortButton_->hide();
	message_ = new QTextEdit;
	message_->hide();

	sslServer_ = new QSslServer(this);

	connect(connectButton_, SIGNAL(clicked()), this, SLOT(openSession()));
	connect(sslServer_, SIGNAL(newConnection()), this, SLOT(connectToClient()));
	connect(changePortButton_, SIGNAL(clicked()), this, SLOT(switchPorts()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(changePortButton_);
	buttonLayout->addWidget(connectButton_);
	buttonLayout->addWidget(sendButton_);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(status_, 0, 0, 1, 2);
	mainLayout->addWidget(portLabel_, 1, 0);
	mainLayout->addWidget(port_, 1, 1);
	mainLayout->addWidget(message_, 2, 0, 1, 2);
	mainLayout->addLayout(buttonLayout, 3, 0, 1, 2);
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
	}

	status_->setText(tr("The server is running on port %1 at address %2.")
						.arg(sslServer_->serverPort())
						.arg(sslServer_->serverAddress().toString()));

	connectButton_->hide();
	portLabel_->hide();
	port_->hide();
	changePortButton_->show();
}

void SslServer::connectToClient()
{
//	QSslSocket *clientConnection = sslServer_->nextPendingConnection();
}

void SslServer::sendMessage()
{

}

void SslServer::switchPorts()
{
	changePortButton_->hide();
	sendButton_->hide();

	status_->setText(tr("Listen on new port:"));
	portLabel_->show();
	port_->show();
	connectButton_->show();
}

