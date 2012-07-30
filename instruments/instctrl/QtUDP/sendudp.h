#ifndef SENDUDP_H
#define SENDUDP_H

#include <QtNetwork>
#include <sys/time.h>

class SendUDP : public QObject
{
	Q_OBJECT;

public:
	SendUDP();

protected slots:
	void newData();

private:
	QUdpSocket *udp;
	char readBuffer[65000];
	char writeBuffer[65000];
};

#endif /*SENDUDP_H*/
