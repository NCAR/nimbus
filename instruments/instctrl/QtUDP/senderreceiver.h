#ifndef SENDERRECEIVER_H
#define SENDERRECEIVER_H

#include <QtGui>
#include <QtNetwork>

/**
 * Can send and receive messages in the form of UDP datagrams.
 * These messages are sent and received from various IP
 * addresses on a specified UDP port.
 */
class SenderReceiver : public QDialog
{
	Q_OBJECT;

public:
	SenderReceiver(QWidget *parent = 0);

public slots:
	void readMode();
	void readPendingDatagrams();
	void writeMode();
	void writeToOne();
	void writeToAll();
	void writeNewDatagram();
	void broadcastDatagrams();
	void switchPortMode();

private:
	QUdpSocket *udpSocket_;
	QPushButton *writeButton_;
	QPushButton *readButton_;
	QPushButton *singleSendButton_;
	QPushButton *multiSendButton_;
	QPushButton *changePortButton_;
	QAction *singleSend_;
	QAction *broadcastSend_;
	QComboBox *port_;
	QComboBox *address_;
	QSet<QString> *uniquePorts_;
	QSet<QString> *uniqueAddresses_;
	QTextEdit *message_;
	QLabel *connect_;
	QLabel *mode_;
	QLabel *portLabel_;
	QLabel *addressLabel_;
	QLabel *messageLabel_;
	QPushButton *writeMode_;
};

#endif /*SENDERRECEIVER_H*/
