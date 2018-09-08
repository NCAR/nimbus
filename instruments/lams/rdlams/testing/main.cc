#include <QApplication>

#include "snd_udp.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv, false);

  SendUDP sndr;

  app.exec();
 
}
