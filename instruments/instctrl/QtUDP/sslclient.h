#ifndef SSLCLIENT_H
#define SSLCLIENT_H

#include <QtGui>
#include <QtNetwork>

class SslClient : public QDialog
{
	Q_OBJECT;

public:
	SslClient(QWidget *parent = 0);

private slots:
	void connectToServer();
	void clientConnected();
	void clientEncrypted();
	void clientDisconnected();
	void sendMode();
	void sendMessage();
	void readMode();
	void switchHostMode();
	void quitSession();
	void displayError(QAbstractSocket::SocketError socketError);
	void displayError(const QList<QSslError> & errors);
	void slot_stateChanged(QAbstractSocket::SocketState);

private:
	QSslSocket *sslSocket_;

	QLabel *status_;
	QLabel *connection_;
	QLabel *portLabel_;
	QLabel *hostLabel_;
	QLabel *clientLabel_;

	QComboBox *port_;
	QSet<QString> *uniquePorts_;
	QLineEdit *hostName_;
	QLineEdit *clientName_;
	QTextEdit *message_;
	QPushButton *connectButton_;
	QPushButton *sendButton_;
	QPushButton *switchButton_;
	QPushButton *writeButton_;
	QPushButton *quitButton_;
};

#endif /*SSLCLIENT_H*/
