#include <cstdio>
#include <unistd.h>
#include <QDateTime>
#include "sendudp.h"

#include <sys/types.h>
#include <sys/socket.h>


/* -------------------------------------------------------------------- */
SendUDP::SendUDP(bool encrypt)
{
        _encrypt = encrypt;

	udp = new QUdpSocket();

	status_ = new QLabel(tr("Enter port and key to send:"));
        connection_ = new QLabel;
        connection_->hide();
        portLabel_ = new QLabel(tr("Port:"));
        clientLabel_ = new QLabel(tr("Instrument Key:"));

        _port = new QLineEdit;
        _instkey = new QLineEdit;

        _startButton = new QPushButton(tr("Start"));
        _quitButton = new QPushButton(tr("Quit"));

        connect(_startButton, SIGNAL(clicked()), this, SLOT(startSendingPackets()));
        connect(_quitButton, SIGNAL(clicked()), this, SLOT(quitSession()));
	connect(udp, SIGNAL(readyRead()), this, SLOT(newData()));

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(_startButton);
        buttonLayout->addWidget(_quitButton);

        QGridLayout *mainLayout = new QGridLayout;
        mainLayout->addWidget(connection_, 0, 0, 1, 2);
        mainLayout->addWidget(status_, 1, 0, 1, 2);
        mainLayout->addWidget(portLabel_, 2, 0);
        mainLayout->addWidget(_port, 2, 1);
        mainLayout->addWidget(clientLabel_, 3, 0);
	mainLayout->addWidget(_instkey, 3, 1);
        mainLayout->addLayout(buttonLayout, 4, 0, 1, 2);
	setLayout(mainLayout);
        if (encrypt)
           setWindowTitle(tr("Aircraft"));
        else
           setWindowTitle(tr("Instrument"));


}

void SendUDP::startSendingPackets()
{
	QHostAddress addr("0.0.0.0");	// Stephanie, use localhost addr
	int rc;
	QString portString = _port->text();
        bool validPort;
        int portNumber = portString.toInt(&validPort);

        if (!validPort || portNumber > 65535 || portNumber < 0) {
                status_->setText(tr("Invalid port number. Retry:"));
                return;
        }

	while (1) {
	QDateTime t = QDateTime::currentDateTime();

	sprintf(writeBuffer, "%s,%s,command", _instkey->text().toStdString().c_str(), t.toString(QString("yyyyMMddThhmmss")).toStdString().c_str());
	printf("%s\n", writeBuffer);
        status_->setText(QString(writeBuffer));
        this->show();
	if ((rc = udp->writeDatagram(writeBuffer, strlen(writeBuffer), 
                                     addr, portNumber)) <= 0)
		printf("  return= %d\n", rc);
	//status_->setWordWrap(true);
        //status_->show();
	sleep(3);
	}
}

void SendUDP::newData()
{
	int nBytes = udp->readDatagram(readBuffer, 65000);

	readBuffer[nBytes] = '\0';
//	printf("nBytes=%d, %s\n", nBytes, buffer);

	readBuffer[80] = '\0';
	printf("%s\n", readBuffer);
}

void SendUDP::quitSession()
{
   close();
}
