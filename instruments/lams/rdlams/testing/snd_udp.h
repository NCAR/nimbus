#include <QUdpSocket>
#include <sys/time.h>

class SendUDP
{
public:
  SendUDP();

private:
  QUdpSocket		*udp;
  long			buffer[512]={0};
};

