#include "transmit.h"
#include <ctime>
#include <zlib.h>
#include <sys/param.h>

sqlTransmit::sqlTransmit(const std::string ac) : _aircraft(ac)
{
  _timeInterval = 0;
  _counter = 0;
  _packetCounter = 0;
  _q = _aircraft + "\nsql\n";
}

sqlTransmit::~sqlTransmit()
{
  if (_counter > 0)
    sendString(_q);
}

void sqlTransmit::queueString(const std::string& str)
{
  _q += str;
  _q += "\n";
  ++_counter;

  if (timeInterval() > 0 && _counter > timeInterval())
  {
    sendString(_q);
    _counter = 0;
    _q = _aircraft + "\nsql\n";
  }
}

void sqlTransmit::sendString(const std::string& str)
{
  char fName[MAXPATHLEN], *dir;

  if ((dir = getenv("XMIT_DIR")) == 0)
  {
    fprintf(stderr, "env XMIT_DIR undefined, fatal for '-x'\n");
    exit(1);
  }

  {
  time_t t = time(0);
  char timeStamp[64];
  strftime(timeStamp, sizeof(timeStamp), "%Y-%m-%dT%H:%M:%S", gmtime(&t));

  sprintf(fName, "%s/%s_nimbus_start_%s.gz", dir, _aircraft.c_str(), timeStamp);
  }
//  sprintf(fName, "%s/nimbus_sql_%s_%05d.gz", dir, _aircraft.c_str(), _packetCounter++);

  gzFile gzfd = gzopen(fName, "w+");

  gzwrite(gzfd, str.c_str(), str.length());
  gzclose(gzfd);

  char command[256];
  sprintf(command, "/home/local/bin/pqinsert %s", fName);
  system(command);
}
