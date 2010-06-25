#include "nimbus.h"
#include "transmit.h"
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <zlib.h>
#include <sys/param.h>

void quit();

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
    quit();
  }

  {
  time_t t = time(0);
  char timeStamp[64];
  strftime(timeStamp, sizeof(timeStamp), ISO8601_T, gmtime(&t));

  sprintf(fName, "%s/%s_nimbus_start_%s.gz", dir, _aircraft.c_str(), timeStamp);
  }

  gzFile gzfd = gzopen(fName, "w+");

  gzwrite(gzfd, str.c_str(), str.length());
  gzclose(gzfd);

  pid_t pid = fork();

  if (pid == 0)
  {
    const char * command = 0;
    if (cfg.GroundFeedType() == Config::UDP)
      command = "/home/local/Systems/scripts/sendSQL";
    if (cfg.GroundFeedType() == Config::LDM)
      command = "pqinsert";

    if (execlp((const char *)command, (const char *)command, (const char *)fName, (const char *)0) == -1)
    {
      fprintf(stderr, "nimbus:transmit.cc: Failed to execute pqinsert, errno = %d\n", errno);
      _exit(1);
    }
    _exit(0);
  }
}
