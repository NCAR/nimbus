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
	void writeToOne();
	void writeToAll();
	void writeNewDatagram();
	void broadcastDatagrams();

private:
	QUdpSocket *udpSocket_;
	QPushButton *writeButton_;
	QPushButton *singleSendButton_;
	QPushButton *multiSendButton_;
	QAction *singleSend_;
	QAction *broadcastSend_;
	QComboBox *address_;
	QTextEdit *message_;
	QLabel *mode_;
	QLabel *addressLabel_;
	QLabel *messageLabel_;
	QPushButton *writeMode_;
};

#endif /*SENDERRECEIVER_H*/
