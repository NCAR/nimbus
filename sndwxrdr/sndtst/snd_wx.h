#ifndef SendUDP_H
#define SendUDP_H
#include <QObject>
#include "/usr/lib/qt4/include/QtNetwork/qudpsocket.h"
#include <sys/time.h>

class SendUDP : public QObject
{
  Q_OBJECT

public:
  SendUDP();

private:
  QUdpSocket           *udp;
  struct timeval	tv;
  double		ts, pts;

};
#endif
