#ifndef SENDERRECEIVER_H
#define SENDERRECEIVER_H

#include <QtGui>
#include <QtNetwork/QUdpSocket>

/**
 * Can send and receive messages in the form of UDP datagrams.
 * These messages are sent and received from various IP
 * addresses on a specific UDP port.
 */
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
	QTextEdit *message_;
	QLabel *mode_;
};

#endif /*SENDERRECEIVER_H*/
