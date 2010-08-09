#include <qhostaddress.h>
#include <qsocketdevice.h>
#include <sys/time.h>

class SendUDP
{
public:
  SendUDP();

private:
  QSocketDevice		*udp;
  struct timeval	tv;
  double		ts, pts;
  char			buffer[65000];

};

