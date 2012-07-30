#ifndef SWITCH_H
#define SWITCH_H

#include <QtGui>
#include <QtNetwork>
#include "qsslserver.h"

class Switch : public QDialog
{
	Q_OBJECT;

public:
	Switch(QWidget *parent = 0);

private slots:
	void openSession();
	void connectToClient();
	void addClientName();
	void clientConnected();
	void clientEncrypted();
	void clientDisconnected();
	void readMode();
	void sendToClient();
	void chooseClient();
	void showDatagrams();
	void switchPorts();
	void quitSession();

private:
	QSslServer *sslServer_;
	QMap<QString, QSslSocket *> connectedSockets_;
	QMap<QSslSocket *, QList<QByteArray> > udpLists_;
	QUdpSocket *udpSocket_;

	QLabel *status_;
	QLabel *connection_;
	QLabel *connectedClient_;
	QLabel *portLabel_;
	QLabel *clientsLabel_;

	QComboBox *port_;
	QSet<QString> *uniquePorts_;
	QComboBox *clients_;
	QPushButton *connectButton_;
	QPushButton *chooseClientButton_;
	QPushButton *showDatagramsButton_;
	QPushButton *changePortButton_;
	QPushButton *quitButton_;
};

#endif /*SWITCH_H*/
