#include <QApplication>

#include "rd_udp.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv, false);

  RdUDP	rdr;

  app.exec();
 
}
