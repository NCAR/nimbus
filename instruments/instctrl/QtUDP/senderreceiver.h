#ifndef SENDERRECEIVER_H
#define SENDERRECEIVER_H

#include <QtGui>
#include <QtNetwork/QUdpSocket>

class SenderReceiver : public QDialog
{
	Q_OBJECT;

public:
	SenderReceiver(QWidget *parent = 0);

public slots:
	void readPendingDatagrams();
	void writeMode();
	void writeNewDatagram();

private:
	QUdpSocket *udpSocket_;
	QPushButton *writeButton_;
	QPushButton *sendButton_;
//	QPushButton *readButton_;
//	QLineEdit *address_;
	QTextEdit *message_;
	QLabel *mode_;
};

#endif /*SENDERRECEIVER_H*/
