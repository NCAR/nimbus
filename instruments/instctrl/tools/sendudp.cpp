#include <cstdio>
#include <unistd.h>
#include <QDateTime>
#include "sendudp.h"
#include <stdio.h>
#include <string.h>

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
        if (_encrypt) {
           setWindowTitle(tr("Aircraft"));
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
                           // this object will encrypt
                           QCA::Encode,
                           *_key, *_iv);
        }
        else
           setWindowTitle(tr("Instrument"));


}

void SendUDP::startSendingPackets()
{
	//QHostAddress addr("0.0.0.0");	// Stephanie, use localhost addr
	QHostAddress addr("127.0.0.1");	// Stephanie, use localhost addr
	int rc;
	QString portString = _port->text();
        bool validPort;
        int portNumber = portString.toInt(&validPort);
	bool encryptFail = false;

        if (!validPort || portNumber > 65535 || portNumber < 0) {
                status_->setText(tr("Invalid port number. Retry:"));
                return;
        }

	while (1) {
	   QDateTime t = QDateTime::currentDateTime();

	   if (_encrypt)
	      sprintf(writeBuffer, "%s,%s,status", 
                        _instkey->text().toStdString().c_str(), 
                        t.toString(QString("yyyyMMddThhmmss")).toStdString().c_str());
	   else
	      sprintf(writeBuffer, "%s,%s,command", 
                         _instkey->text().toStdString().c_str(), 
                         t.toString(QString("yyyyMMddThhmmss")).toStdString().c_str());

	   printf("%s\n", writeBuffer);
           status_->setText(QString(writeBuffer));
           this->show();

	   // If we're set up to mimic the aircraft, encrypt the datagram before sending it
	   if (_encrypt) {

           _cipher = new QCA::Cipher(QString("aes128"),QCA::Cipher::CBC,
                           // use Default padding, which is equivalent to PKCS7 for CBC
                           QCA::Cipher::DefaultPadding,
                           // this object will encrypt
                           QCA::Encode,
                           *_key, *_iv);

              //char * ch_arg = (char *) malloc (strlen(writeBuffer) +1);
              //strncpy(ch_arg, writeBuffer, strlen(writeBuffer));
              //ch_arg[strlen(writeBuffer)] = '\0';
printf("About to create SecureArray using ch_arg: %s\n", writeBuffer);
	      //QCA::SecureArray arg = ch_arg;
//printf("About to call cipher->update using arg: %s\n", arg.data());
	      QCA::SecureArray u = _cipher->update(writeBuffer);
	      if (!_cipher->ok()) {
	         encryptFail=true;
                 printf("ERROR:Encryption of previous message Failed! (update)\n");
	      } else {
	         QCA::SecureArray f = _cipher->final();
                 if (!_cipher->ok()) {
	            encryptFail=true;
	            printf("ERROR:Encryption of previous message Failed! (final)\n");
                 } else { // All is ok with encryption
	            sprintf(writeBuffer, "%s%s", u.data(), f.data());
	            printf("Encoded message: %s\n", writeBuffer);
                 }
              }
	   }

	   if (!encryptFail) 
	      if ((rc = udp->writeDatagram(writeBuffer, strlen(writeBuffer), 
                                           addr, portNumber)) <= 0)
	         printf("ERROR:writeDatagram failed with return value= %d\n", rc);
	   //status_->setWordWrap(true);
           //status_->show();
	   encryptFail = false;
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
