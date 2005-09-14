#include "transmit.h"
#include <zlib.h>

sqlTransmit::sqlTransmit()
{
  _timeInterval = 5;
  _counter = 0;
  _packetCounter = 0;

  _q = "BEGIN;";
}

sqlTransmit::~sqlTransmit()
{
  if (_counter > 0)
    sendString(_q);
}

void sqlTransmit::queueString(const std::string& str)
{
  _q += str;

  if (++_counter > timeInterval())
  {
    _q += "COMMIT;";
    sendString(_q);
    _counter = 0;
    _q = "BEGIN;";
  }
}

void sqlTransmit::sendString(const std::string& str)
{
  char fName[256];
  sprintf(fName, "sql_sequence%d.gz", _packetCounter++);

  gzFile gzfd = gzopen(fName, "w+");

  gzwrite(gzfd, str.c_str(), str.length());
  gzclose(gzfd);
}
