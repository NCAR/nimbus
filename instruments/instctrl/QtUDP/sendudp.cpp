#include <cstdio>
#include <unistd.h>
#include <QDateTime>
#include "sendudp.h"

#include <sys/types.h>
#include <sys/socket.h>

static const int port = 33500;	// Port we have chosen to use.

/* -------------------------------------------------------------------- */
SendUDP::SendUDP()
{
	int rc;
	udp = new QUdpSocket();
	QHostAddress addr("128.117.82.6");	// Stephanie, use localhost addr

	connect(udp, SIGNAL(readyRead()), this, SLOT(newData()));

	while (1) {
	QDateTime t = QDateTime::currentDateTime();

	sprintf(writeBuffer, "EOLR,%s,command", t.toString(QString("yyyyMMddThhmmss")).toStdString().c_str());
	printf("%s\n", writeBuffer);
	if ((rc = udp->writeDatagram(writeBuffer, strlen(writeBuffer), addr, port)) <= 0)
		printf("  return= %d\n", rc);

	sleep(3);
	}
}

void SendUDP::newData()
{
	int nBytes = udp->readDatagram(readBuffer, 65000);

	readBuffer[nBytes] = '\0';
//	printf("nBytes=%d, %s\n", nBytes, buffer);

	readBuffer[80] = '\0';
	printf("%s\n", readBuffer);
}

