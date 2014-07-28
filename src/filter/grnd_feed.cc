/*
-------------------------------------------------------------------------
OBJECT NAME:	grnd_feed.cc

FULL NAME:	UDP broadcast for Ground Feed.

DESCRIPTION:	Class to produce 1 per second ascii string to broadcast
		to ground, and send it to the dest host.

INPUT:		${PROJ_DIR}/###/groundvars

COPYRIGHT:	University Corporation for Atmospheric Research, 2005-08
-------------------------------------------------------------------------
*/

#include "nimbus.h"
#include "decode.h"
#include "grnd_feed.h"
#include "md5.h"

#include <unistd.h>  // gethostname()
#include <sstream>
#include <bzlib.h>
#include <sys/stat.h>

// eol-rt-data.fl-ext.ucar.edu
const std::string GroundFeed::DEST_HOST_ADDR = "128.117.188.122";

using namespace nidas::util;

std::string
GroundFeed::
getDestHostName()
{
  char hostname[128];
  if ((gethostname(hostname, sizeof(hostname)) == 0) &&
      strncmp(hostname, "acserver", 8) == 0)
  {
    return DEST_HOST_ADDR;
  }
  std::cerr << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n"
	    << "WARNING: GroundFeed DID NOT DETECT HOST 'acserver',\n"
	    << "MESSAGES WILL BE SENT TO localhost\n"
	    << "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n";
  return "127.0.0.1";
}


/* -------------------------------------------------------------------- */
GroundFeed::
GroundFeed(int rate) : 
  UDP_Base(31007), 
  _dataRate(rate)
{
  // maintain the 5-minute count from earlier implementations
  _default_latch_seconds = 300;
  ILOG(("GroundFeed ctor"));

  if (cfg.GroundFeedType() != Config::UDP)
    return;

  std::string fileName(XMIT_VARS);
  fileName += ".rt";
  readFile(fileName);

  setCoordinatesFrom(_varList);

  _socket = new nidas::util::MulticastSocket;
  _to = new Inet4SocketAddress(Inet4Address::getByName(getDestHostName()), UDP_PORT);

  // compute checksum for this flight, store in cfg
  cfg.SetChecksum(calculateChecksum());
}

/* -------------------------------------------------------------------- */
void GroundFeed::setCoordinatesFrom(const std::vector<var_base *> & list) const
{
  std::vector<var_base *>::const_iterator it;
  for (it = list.begin(); it != list.end(); ++it)
  {
    if (strstr((*it)->name, "LON"))
      cfg.SetCoordLON((*it)->name);
    if (strstr((*it)->name, "LAT"))
      cfg.SetCoordLAT((*it)->name);
    // Don't set it to PALTF, we don't want an altitude in feet as the reference.
    if (strstr((*it)->name, "ALT") && strstr((*it)->name, "ALTF") == 0)
      cfg.SetCoordALT((*it)->name);
  }
}


/* -------------------------------------------------------------------- */
void GroundFeed::BroadcastData(nidas::core::dsm_time_t tt)
	throw(IOException)
{
  static int rate_cntr = 0;
  static bool valid_ground_conn = false;

  if (cfg.GroundFeedType() != Config::UDP)
    return;
    
  rate_cntr++;

  std::string timeStamp = formatTimestamp(tt);

  // Only send data if ground connection is verified
  struct stat stFileInfo;
  std::string noconnFile("");
  if ( !valid_ground_conn  ) {
    noconnFile += getenv("XMIT_DIR");
    noconnFile += "/noconn";
    valid_ground_conn = stat(noconnFile.c_str(), &stFileInfo) != 0;
  }

  // Compose the string to send to the ground
  std::stringstream groundString;
  if (cfg.Aircraft() == Config::HIAPER)
    groundString << "GV";
  if (cfg.Aircraft() == Config::NRL_P3)
    groundString << "NRLP3";
  if (cfg.Aircraft() == Config::C130)
    groundString << "C130";

  groundString << "," << timeStamp;

  updateData(tt);

  // Format data
  for (size_t i = 0; i < _varList.size(); ++i)
  {
    groundString << "," << formatVariable(i);
  }
  groundString << "\n";

  // Only send every so often.
  if ((rate_cntr % _dataRate) != 0)
    return;

  // compress the stream before sending it to the ground
  char buffer[32000];
  memset(buffer, 0, 32000);
  unsigned int bufLen = sizeof(buffer);
  int ret = BZ2_bzBuffToBuffCompress( buffer, &bufLen, (char *) groundString.str().c_str(),
                                      groundString.str().length(),9,0,0);
  if (ret < 0) {
    typedef struct {     // copied from bzlib.c
      FILE*     handle;
      char      buf[BZ_MAX_UNUSED];
      int       bufN;
      bool      writing;
      bz_stream strm;
      int       lastErr;
      bool      initialisedOk;
    } bzFile;
    bzFile b;
    b.lastErr = ret;
    int errnum;
    char msg[100];
    sprintf(msg, "Failed to compress the ground feed stream: %s\n", BZ2_bzerror(&b, &errnum) );
    ::LogMessage(msg);
    return;
  }

  try {
    _socket->sendto(buffer, bufLen, 0, *_to);
  }
  catch (const nidas::util::IOException& e) {
    fprintf(stderr, "nimbus::GroundFeed: %s\n", e.what());
  }

//  printf("\ncompressed %d -> %d\n", groundString.str().length(), bufLen);
  printf("GroundFeed: %s\n", groundString.str().c_str());
}

/* -------------------------------------------------------------------- */
std::string GroundFeed::calculateChecksum() {

  if (_varList.size() == 0) return "0";

  //calculate current md5 using proj, platform, flight# and var list.
  std::string md5_instring = cfg.ProjectName() + cfg.TailNumber() 
    + cfg.FlightNumber();

  for (unsigned int i=0; i<_varList.size(); i++)
    md5_instring += _varList[i]->name;

  return md5(md5_instring);
}

