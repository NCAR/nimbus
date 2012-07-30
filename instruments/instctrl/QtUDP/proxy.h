#ifndef PROXY_H
#define PROXY_H

#include <QtGui>
#include <QtNetwork>

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
	void slot_stateChanged(QAbstractSocket::SocketState);

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
