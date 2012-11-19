#ifndef QSSLSERVER_H
#define QSSLSERVER_H

#include <QtNetwork>
#include <QtCore>

/**
 * QTcpServer that has SSL support. Can connect debugging messages
 * to observe certificate verification process through signals.
 */

class QSslServer : public QTcpServer
{
	Q_OBJECT;

public:
	QSslServer(QObject * parent = 0);
	virtual ~QSslSocket *nextPendingConnection();

protected:
	void incomingConnection(int socketDescriptor);

private:
	QList<QSslSocket *> pendingConnections_;
	void connectDebuggingMessages(QSslSocket * serverSocket);

private slots:
	void slot_encrypted ();
	void slot_encryptedBytesWritten (qint64 written);
	void slot_modeChanged (QSslSocket::SslMode mode);
	void slot_peerVerifyError (const QSslError &error);
	void slot_sslErrors (const QList<QSslError> &errors);

	void slot_connected ();
	void slot_disconnected ();
	void slot_error (QAbstractSocket::SocketError);
	void slot_hostFound ();
	void slot_proxyAuthenticationRequired (const QNetworkProxy &, QAuthenticator *);
	void slot_stateChanged (QAbstractSocket::SocketState);

	void slot_readyRead ();
};

#endif /*QSSLSERVER_H*/

