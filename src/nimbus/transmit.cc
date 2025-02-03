/* This class is currently deprecated / unused.
 */
#include "nimbus.h"
#include "transmit.h"
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <zlib.h>
#include <sys/param.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


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

  snprintf(fName, MAXPATHLEN, "%s/%s_nimbus_start_%s.gz", dir, _aircraft.c_str(), timeStamp);
  }

  gzFile gzfd = gzopen(fName, "wb");

  gzwrite(gzfd, str.c_str(), str.length());
  gzclose(gzfd);

  pid_t pid = fork();

  if (pid == 0)
  {
    char command[256];
    command[0] = 0;
    if (cfg.GroundFeedType() == Config::UDP)
    {
      strcpy(command, cfg.ProjectDirectory().c_str());
      strcat(command, "/scripts/sendSQL");
    }
    if (cfg.GroundFeedType() == Config::LDM)
      strcpy(command, "pqinsert");

    // Set up logfile in /tmp
    int fd = open("/tmp/sendSQL.log", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    dup2(fd, fileno(stdout));   // make stdout go to file
    dup2(fd, fileno(stderr));   // make stderr go to file
    close(fd);     // fd no longer needed - the dup'ed handles are sufficient

    if (execlp((const char *)command, (const char *)command, (const char *)fName, (const char *)0) == -1)
    {
      fprintf(stderr, "nimbus:transmit.cc: Failed to execute pqinsert, errno = %d\n", errno);
      _exit(1);
    }
    _exit(0);
  }
}
