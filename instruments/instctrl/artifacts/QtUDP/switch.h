#ifndef SWITCH_H
#define SWITCH_H

#include <QtGui>
#include <QtNetwork>
#include "qsslserver.h"

/**
 * The server application for the remote instrument control project, to be run on
 * eol-rt-data over port 80. Logs datagrams sent by the proxy client over a secure
 * connection. Datagrams received directly from the aircraft server are identified
 * by their instrument keys and sent to their respective clients.
 */

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
	struct ProxyClient {
		QString instKey_;
		int portNumber_;
		QHostAddress ipAddress_;

	public:
		bool operator<(const ProxyClient& rhs) const {return instKey_ < rhs.instKey_;}
	};

	QSslServer *sslServer_;
	QMap<ProxyClient, QSslSocket *> connectedSockets_;
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
