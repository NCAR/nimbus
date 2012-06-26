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
	void addClientName();
	void clientConnected();
	void clientEncrypted();
	void clientDisconnected();
	void sendMode();
	void sendToClient();
	void sendMessage();
	void broadcastMessage();
	void readMode();
	void switchPorts();
	void quitSession();

private:
	QSslServer *sslServer_;
	QMap<QString, QSslSocket *> connectedSockets_;

	QLabel *status_;
	QLabel *connection_;
	QLabel *connectedClient_;
	QLabel *portLabel_;
	QLabel *clientsLabel_;

	QComboBox *port_;
	QSet<QString> *uniquePorts_;
	QComboBox *clients_;
	QPushButton *connectButton_;
	QPushButton *sendButton_;
	QPushButton *sendToClientButton_;
	QPushButton *broadcastButton_;
	QPushButton *writeButton_;
	QPushButton *changePortButton_;
	QPushButton *quitButton_;
	QTextEdit *message_;
};

#endif /*SSLSERVER_H*/
