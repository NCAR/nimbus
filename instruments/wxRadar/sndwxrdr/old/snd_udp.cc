#include <cstdio>
#include <unistd.h>
#include <qdatetime.h>
#include "snd_udp.h"

#include <sys/types.h>
#include <sys/socket.h>

static const int port = 7071;	// Port we have chosen to use.

/* -------------------------------------------------------------------- */
SendUDP::SendUDP()
{
  int rc;
  udp = new QSocketDevice(QSocketDevice::Datagram);
  QHostAddress	host;

  host.setAddress("128.117.84.255");	// Inhouse
//  host.setAddress("192.168.84.255");	// Onboard

  // Hack, because QSocketDevoce doesn't support broadcast correctly.
  int opt = 1;
  setsockopt(udp->socket(), SOL_SOCKET, SO_BROADCAST, &opt, sizeof(int));

  while (1)
  {
    QDateTime t = QDateTime::currentDateTime();

    sprintf(buffer, "IWG1,%s,39.2348,-105.2398,1506.5,1506.4,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,25.34\r\n", t.toString(Qt::ISODate).ascii());
    printf(buffer);
    if ((rc = udp->writeBlock(buffer, strlen(buffer), host, port)) <= 0)
      printf("return= %d\n", rc);

    sleep(1);
  }
}
