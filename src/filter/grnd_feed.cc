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

#include <sstream>
#include <bzlib.h>
#include <sys/stat.h>

//const int GroundFeed::GRND_UDP_PORT = 31007;
// eol-rt-data.guest.ucar.edu
const std::string GroundFeed::DEST_HOST_ADDR = "128.117.188.122";

using namespace nidas::util;


/* -------------------------------------------------------------------- */
GroundFeed::GroundFeed(int rate) : UDP_Base(31007), _dataRate(rate)
{
  ILOG(("GroundFeed ctor"));

  if (cfg.GroundFeedType() != Config::UDP)
    return;

  std::string fileName(XMIT_VARS);
  fileName += ".rt";
  _varList = readFile(fileName);

  setCoordinatesFrom(_varList);

  _socket = new nidas::util::DatagramSocket;
  _to = new Inet4SocketAddress(Inet4Address::getByName(DEST_HOST_ADDR), UDP_PORT);

  // Initialize vectors used for averaging
  for (size_t i = 0; i < _varList.size(); ++i)
  {
     for (size_t j = 0; j < _varList[i]->Length; j++)
     {
       _summedData.push_back(0.0);
       _lastGoodData.push_back(-32767);
       _summedDataCount.push_back(0);
       _lastGoodDataIncrement.push_back(0);
     }
  }

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
void GroundFeed::BroadcastData(const std::string & timeStamp)
	throw(IOException)
{
  static int rate_cntr = 0;
  static bool valid_ground_conn = false;

  if (cfg.GroundFeedType() != Config::UDP)
    return;
    
  rate_cntr++;

  extern NR_TYPE * AveragedData;

  // Accumulate data so we can send an average.
  size_t counter = 0;
  for (size_t i = 0; i < _varList.size(); ++i)
  {
    // @todo when legacy zeroth bin goes away, then start with j = 0.
    size_t s = _varList[i]->Length > 1 ? 1 : 0;
    for (size_t j = s; j < _varList[i]->Length; j++)
    {
      if (!isnan(AveragedData[(_varList[i]->LRstart)+j]))
      { 
        _summedData[counter] += AveragedData[(_varList[i]->LRstart)+j];
        _summedDataCount[counter]++;
        counter++;
      }
      else
      {
        counter++;
      }
    }
  }

  // Only send every so often.
  if ((rate_cntr % _dataRate) != 0)
    return;

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

  counter = 0;
  for (size_t i = 0; i < _varList.size(); ++i)
  {
    if (_varList[i]->Length > 1) 
    { 
      // Vector data
      // @todo when legacy zeroth bin goes away, then start with j = 0.
      size_t s = 1;
      for (size_t j = s; j < _varList[i]->Length; j++) 
      {
        if (_summedDataCount[counter] > 0)
        {
          _lastGoodData[counter] = _summedData[counter] / _summedDataCount[counter];
          if (j == s) 
            groundString << ",'{" << _lastGoodData[counter];
          else 
            groundString << "," << _lastGoodData[counter];
          _lastGoodDataIncrement[counter] = 0;
        } else
        {
          // Ship last good value unless we've seen NaNs for quite a while (60 increments = 5 minutes if 5 second intervals)
          if (_lastGoodDataIncrement[counter] < 60)
          {
            if (j == s)
              groundString << ",'{" << _lastGoodData[counter];
            else 
              groundString << "," << _lastGoodData[counter];
            _lastGoodDataIncrement[counter]++;
          }
          else
          {
            if (j == s)
              groundString << ",'{" << "-32767";
            else
              groundString << "," << "-32767";
          }
        }
        _summedData[counter] = 0.0;
        _summedDataCount[counter] = 0;
        counter++;
      }
      groundString << "}'";
    } else {
      // Scalar data
      if (_summedDataCount[counter] > 0)
      {
        _lastGoodData[counter] = _summedData[counter] / _summedDataCount[counter];
        groundString << "," << _lastGoodData[counter];
        _lastGoodDataIncrement[counter] = 0;
      }
      else
      {
        // Ship last good data so long as it's been less than 300 seconds (5 min) since last seen.
        if (_lastGoodDataIncrement[counter] < (int) (300/_dataRate))  
        {
          groundString << "," << _lastGoodData[counter];
          _lastGoodDataIncrement[counter]++;
        }
        else
        {
          groundString << "," << "-32767";
        }
      }
      _summedData[counter] = 0.0;
      _summedDataCount[counter] = 0;
      counter++;
    }
  }
  groundString << "\n";

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

