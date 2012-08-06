#ifndef PROXY_H
#define PROXY_H

#include <QtGui>
#include <QtNetwork>

/**
 * The proxy client application for the remote instrument control project. Establishes
 * a secure connection with the server application on eol-rt-data over port 80 and sets
 * up a QUdpSocket to read datagrams sent from the local machine to a chosen port number.
 * The datagrams are sent to the server through the secure connection. Status datagrams
 * that match the entered instrument key are sent back to the Proxy from the server.
 */

class Proxy : public QDialog
{
	Q_OBJECT;

public:
	Proxy();

private slots:
	void connectToServer();
	void clientConnected();
	void clientEncrypted();
	void clientDisconnected();
	void readMode();
	void sendMode();
	void sendToServer();
	void switchHostMode();
	void quitSession();
	void displayError(QAbstractSocket::SocketError socketError);
	void displayError(const QList<QSslError> & errors);

private:
	QUdpSocket *udpInput_;
	QSslSocket *sslSocket_;

	QLabel *status_;
	QLabel *connection_;
	QLabel *portLabel_;
	QLabel *clientLabel_;

	QPushButton *connectButton_;
	QPushButton *switchButton_;
	QPushButton *quitButton_;

	QComboBox *port_;
	QSet<QString> *uniquePorts_;
	QLineEdit *clientName_;
};

#endif /*PROXY_H*/
