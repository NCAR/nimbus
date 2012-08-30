#ifndef SENDUDP_H
#define SENDUDP_H

#include <QtNetwork>
#include <QtGui>
#include <QtCrypto>
#include <sys/time.h>

class SendUDP : public QDialog
{
	Q_OBJECT;

public:
	SendUDP(bool encrypt);

protected slots:
	void newData();
	void startSendingPackets();
        void quitSession();

private:
        bool _encrypt;
	QUdpSocket *udp;
	char readBuffer[65000];
	char writeBuffer[65000];

	// key and iv probably should be read from a file that is 
	// shared between the two switches.
	QCA::SymmetricKey *_key;
	QCA::InitializationVector *_iv;
        QCA::Cipher *_cipher;
	QCA::Initializer *_init;

 	QLabel *status_;
        QLabel *connection_;
        QLabel *portLabel_; 
        QLabel *clientLabel_;

        QLineEdit *_port;
        QLineEdit *_instkey;

        QPushButton *_startButton;
        QPushButton *_quitButton;

};

#endif /*SENDUDP_H*/
