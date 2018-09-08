#include <QUdpSocket>
#include <QHostAddress>
#include <sys/time.h>

class RdUDP : public QObject
{
  Q_OBJECT

public:
  RdUDP();

protected slots:
  void	newData();

private:
  QUdpSocket		*udp;
  char			buffer[65000];
  int			delay[100];
  size_t		 _beam;
  time_t 		rawtime;
  double 		pts;


};

