#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QtGui>
#include <QtNetwork>
#include "qsslserver.h"

class SslServer : public QDialog
{
	Q_OBJECT;

public:
	SslServer(QWidget *parent = 0);

private slots:
	void openSession();
	void connectToClient();
	void clientConnected();
	void clientEncrypted();
	void clientDisconnected();
	void sendMode();
	void sendMessage();
	void readMode();
	void switchPorts();

private:
	QSslServer *sslServer_;
	QSslSocket *currentSocket_;

	QLabel *status_;
	QLabel *connection_;
	QLabel *portLabel_;

	QComboBox *port_;
	QSet<QString> *uniquePorts_;
	QPushButton *connectButton_;
	QPushButton *sendButton_;
	QPushButton *writeButton_;
	QPushButton *changePortButton_;
	QTextEdit *message_;
};

#endif /*SSLSERVER_H*/
