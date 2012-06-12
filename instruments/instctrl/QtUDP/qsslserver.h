#ifndef QSSLSERVER_H
#define QSSLSERVER_H

#include <QtNetwork>
#include <QtCore>

class QSslServer : public QTcpServer
{
	Q_OBJECT;

public:
	QSslServer(QObject * parent = 0);

protected:
	void incomingConnection(int socketDescriptor);
};

#endif /*QSSLSERVER_H*/

