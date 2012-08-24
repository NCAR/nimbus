#ifndef SENDUDP_H
#define SENDUDP_H

#include <QtNetwork>
#include <QtGui>
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
